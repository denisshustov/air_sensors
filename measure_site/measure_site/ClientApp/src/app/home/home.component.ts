import { Component, } from '@angular/core';
import { HttpClient } from '@angular/common/http';
import * as moment from 'moment';
import 'lodash';
import { ChartDataSets, ChartOptions } from 'chart.js';
import { Color, Label } from 'ng2-charts';
import { IMyDpOptions, IMyDateModel } from 'mydatepicker';

declare var _: any;

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
})
export class HomeComponent {

  constructor(private http: HttpClient) { }

  mhz19_co2ChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  ds18b20_tempChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  ms5611_pressue_tempChartData: ChartDataSets[] = [
    { data: [], label: '' },
  ];
  ms5611_temp_tempChartData: ChartDataSets[] = [
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
  step = 1440;
  currentPage = 1;

  myDatePickerOptions: IMyDpOptions = {
    dateFormat: 'dd.mm.yyyy',
  };

  public currentPickerDate: any = { date: { year: 2018, month: 10, day: 9 } };
  onDateChanged(event: IMyDateModel) {
    this.currentPickerDate.year = event.date.year;
    this.currentPickerDate.month = event.date.month;
    this.currentPickerDate.day = event.date.day;
    this.loadData();
  }

  loadData() {
    this.http.get('https://localhost:5001/api/data/dataByDate?date=' + this.currentPickerDate.date.month + '-'
      + this.currentPickerDate.date.day + '-'
      + this.currentPickerDate.date.year).subscribe(data => {

        this.chartLabels = _.take(
          _.drop(
            _.map(data, (x) => moment(x.time_stamp).format('HH:mm')),
            (this.step * this.currentPage) - this.step)
          , this.step);

        this.mhz19_co2ChartData = [
          { data: _.take(_.map(data, (x) => x.mhz19_co2), this.step), label: 'mhz19_co2' },
        ];
        this.ds18b20_tempChartData = [
          { data: _.take(_.map(data, (x) => x.ds18b20_temp), this.step), label: 'ds18b20_temp' },
        ];
        this.ms5611_pressue_tempChartData = [
          { data: _.take(_.map(data, (x) => x.ms5611_pressue), this.step), label: 'ms5611_pressue' },
        ];
        this.ms5611_temp_tempChartData = [
          { data: _.take(_.map(data, (x) => x.ms5611_temp), this.step), label: 'ms5611_temp' },
        ];
      });
  }

  ngOnInit() {
    var currentDate = moment();
    //this.currentPickerDate = { date: { year: currentDate.year(), month: currentDate.month(), day: currentDate.day() } };
    this.currentPickerDate = { date: { year: 2020, month: 5, day: 16 } };
    this.loadData();
  }


}
