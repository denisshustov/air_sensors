import { Component } from '@angular/core';
import { IMyDpOptions, IMyDateModel } from 'mydatepicker';
import { DateServiceService } from '../services/date.service';

@Component({
  selector: 'app-nav-menu',
  templateUrl: './nav-menu.component.html',
  styleUrls: ['./nav-menu.component.css']
})
export class NavMenuComponent {
  public currentPickerDate: any;

  myDatePickerOptions: IMyDpOptions = {
    dateFormat: 'dd.mm.yyyy',
  };
  constructor(private dateService: DateServiceService) { }

  onDateChanged(event: IMyDateModel) {
    this.dateService.DateChangeEvent(event);
  }
  ngOnInit() {
    this.currentPickerDate = this.dateService.GetDate();
  }
}
