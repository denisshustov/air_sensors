import { Component, } from '@angular/core';
import { ChartDataSets, ChartOptions } from 'chart.js';
import { Color, Label } from 'ng2-charts';
import { HttpClient } from '@angular/common/http';
import * as moment from 'moment';

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
})
export class HomeComponent {

  constructor(private http: HttpClient) { }

  lineChartLabels: Label[];// = ['January', 'February', 'March', 'April', 'May', 'June'];
  lineChartData: ChartDataSets[] = [
    { data: [85, 72, 78, 75, 77, 75], label: 'Crude oil prices' },
  ];

  ngOnInit() {
    this.http.get('https://localhost:5001/api/data/data').subscribe(data => {
      var z = data;
      this.lineChartLabels = data.map((x) => moment(x.time_stamp).format('MM/DD/YYYY HH:mm'));
      this.lineChartData.data = data.map((x) => x.ds18b20_temp)
    });
  }

  

  

  lineChartOptions = {
    responsive: true,
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
}
