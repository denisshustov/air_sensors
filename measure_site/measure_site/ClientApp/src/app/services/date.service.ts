import { Injectable, EventEmitter } from "@angular/core";
import * as _ from "lodash";
import * as moment from 'moment';

@Injectable({
  providedIn: 'root'
})
export class DateServiceService {

  public currentPickerDate: any = { date: { year: 2020, month: 5, day: 16 } };

  dateChanged: EventEmitter<any> = new EventEmitter();

  public DateChangeEvent(obj) {
    this.SetDate(obj);
    this.dateChanged.emit(obj);
  }
  public DateChangeEmitter() {
    return this.dateChanged;
  }

  constructor() {

  }

  public GetDate() {
    return this.currentPickerDate;
  }
  public SetDate(value) {
    this.currentPickerDate.date.year = value.date.year;
    this.currentPickerDate.date.month = value.date.month;
    this.currentPickerDate.date.day = value.date.day;
  }
}
