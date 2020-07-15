import { Component, } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import * as moment from 'moment';
import 'lodash';
import { ChartDataSets, ChartOptions } from 'chart.js';
import { Color, Label } from 'ng2-charts';
import { DateServiceService } from '../services/date.service';

declare var _: any;

@Component({
  selector: 'app-second-screen',
  templateUrl: './secondScreen.component.html',
})
export class SecondScreenComponent {

  constructor(private http: HttpClient, private dateService: DateServiceService) { }

  intervalHandler: any;
  mq8ChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  ms5611_temp_tempChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  mq7ChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  mq135ChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  mq3ChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];

  chartLabels: Label[] = [];


  lineChartOptions = {
    responsive: true,
    maintainAspectRatio: false,
  };

  lineChartColors: Color[] = [
    {
      borderColor: 'black',
      backgroundColor: 'rgba(255,255,0,0.28)',
    },
  ];

  lineChartLegend = true;
  lineChartPlugins = [];
  lineChartType = 'line';
  step = 144000;
  currentPage = 1;



  LoadData(d) {
    this.http.post('https://localhost:5001/api/data/dataByDate',
      {
        Date: d
      }
    ).subscribe(data => {

      this.chartLabels = _.take(
        _.drop(
          _.map(data, (x) => moment(x.time_stamp).format('HH:mm')),
          (this.step * this.currentPage) - this.step)
        , this.step);

      this.mq8ChartData = [
        {
          data: _.take(_.map(data, (x) => x.mq8), this.step),
          label: 'mq8',
          pointRadius: 0
        },
      ];
      this.mq7ChartData = [
        {
          data: _.take(_.map(data, (x) => x.mq7), this.step),
          label: 'mq7',
          pointRadius: 0
        },
      ];
      this.mq135ChartData = [
        {
          data: _.take(_.map(data, (x) => x.mq135), this.step),
          label: 'mq135',
          pointRadius: 0
        },
      ];
      this.mq3ChartData = [
        {
          data: _.take(_.map(data, (x) => x.mq3), this.step),
          label: 'mq3',
          pointRadius: 0
        },
      ];
    });
  }

  ngOnInit() {
    //1009x685
    this.dateService.DateChangeEmitter().subscribe(date => this.LoadData(date));
    this.LoadData(this.dateService.GetDate());

    this.intervalHandler = setInterval(() => {
      this.LoadData(this.dateService.GetDate());
    }, 60000 * 10);
  }

  ngOnDestroy() {
    if (this.intervalHandler) {
      clearInterval(this.intervalHandler);
    }
  }
}
