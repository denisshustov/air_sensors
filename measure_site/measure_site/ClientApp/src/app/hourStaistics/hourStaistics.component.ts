import { Component, } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import * as moment from 'moment';
import 'lodash';
import { ChartDataSets, ChartOptions, ChartAnimationOptions, ChartScales, ChartYAxe } from 'chart.js';
import { Color, Label } from 'ng2-charts';
import { DateServiceService } from '../services/date.service';

declare var _: any;

@Component({
  selector: 'app-hour-staistics',
  templateUrl: './hourStaistics.component.html',
})
export class HourStaisticsComponent {

  constructor(private http: HttpClient, private dateService: DateServiceService) { }
  intervalHandler: any;

  mhz19_co2ChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  ds18b20_tempChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  ms5611_pressue_tempChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  am2302_humidChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];

  chartLabels: Label[] = [];


  lineChartOptions = {
    responsive: true,
    maintainAspectRatio: false,
    //animation: <ChartAnimationOptions>{
    //  onComplete: function myfunction(thisArg, args) {
    //    var sourceCanvas = thisArg.chart.ctx.canvas;
    //    var copyWidth = thisArg.scale.xScalePaddingLeft - 5;
    //    var copyHeight = thisArg.scale.endPoint + 5;
    //    var targetCtx = document.getElementById("myChartAxis").getContext("2d");
    //    targetCtx.canvas.width = copyWidth;
    //    targetCtx.drawImage(sourceCanvas, 0, 0, copyWidth, copyHeight, 0, 0, copyWidth, copyHeight);
    //  }
    //},
    //scales: <ChartScales>
    //  {
    //    yAxes: <ChartYAxe>
    //      {
    //        display: true,
    //      ticks: { min: 0, max: 100, stepSize: 100 },
    //      gridLines: { offsetGridLines: false }
    //      }
    //  },

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
  step = 1440;
  currentPage = 1;



  LoadData(d) {
    this.http.get('https://localhost:5001/api/data/dataByLastHour').subscribe(data => {

      this.chartLabels = _.take(
        _.drop(
          _.map(data, (x) => moment(x.time_stamp).format('HH:mm')),
          (this.step * this.currentPage) - this.step)
        , this.step);

      this.mhz19_co2ChartData = [
        {
          data: _.take(_.map(data, (x) => x.mhz19_co2), this.step),
          label: 'mhz19_co2',
          pointRadius: 0
        }
      ];
      this.ds18b20_tempChartData = [
        {
          data: _.take(_.map(data, (x) => x.ds18b20_temp), this.step),
          label: 'ds18b20_temp',
          pointRadius: 0
        },
      ];
      this.ms5611_pressue_tempChartData = [
        {
          data: _.take(_.map(data, (x) => x.ms5611_pressue), this.step),
          label: 'ms5611_pressue',
          pointRadius: 0
        },
      ];
      this.am2302_humidChartData = [
        {
          data: _.take(_.map(data, (x) => x.am2302_humid), this.step),
          label: 'am2302_humid',
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
