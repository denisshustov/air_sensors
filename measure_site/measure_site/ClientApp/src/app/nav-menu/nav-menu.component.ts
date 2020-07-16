import { Component } from '@angular/core';
import { IAngularMyDpOptions, IMyDateModel } from 'angular-mydatepicker';
import { DateServiceService } from '../services/date.service';
import { Router } from '@angular/router';

@Component({
  selector: 'app-nav-menu',
  templateUrl: './nav-menu.component.html',
  styleUrls: ['./nav-menu.component.css']
})
export class NavMenuComponent {
  public currentPickerDate: IMyDateModel;

  myDatePickerOptions: IAngularMyDpOptions = {
    dateFormat: 'dd.mm.yyyy',
    dateRange: false
  };
  constructor(private dateService: DateServiceService, public router: Router) {
    //router.events.subscribe((url: any) => console.log(url));
    //console.log(router.url);  // to print only path eg:"/login"
  }
  gotoDate(addDay) {
    var date = this.dateService.GetDate();
    date.setDate(date.getDate() + addDay);
    this.dateService.DateChangeEvent(date);
    this.currentPickerDate = { isRange: false, singleDate: { jsDate: date }, dateRange: null };
  }

  onDateChanged(event: IMyDateModel) {
    this.dateService.DateChangeEvent(event.singleDate.jsDate);
  }
  ngOnInit() {
    this.currentPickerDate = this.dateService.GetFullDate();
  }
}
