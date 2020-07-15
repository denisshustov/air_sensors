import { Injectable, EventEmitter } from "@angular/core";
import * as _ from "lodash";
import * as moment from 'moment';
import { IMyDateModel, IMySingleDateModel } from "angular-mydatepicker";

@Injectable({
  providedIn: 'root'
})
export class DateServiceService {

  public currentPickerDate: IMyDateModel;

  private ToUTC(date) {
    var now_utc = Date.UTC(date.getUTCFullYear(), date.getUTCMonth(), date.getUTCDate(),
      date.getUTCHours(), date.getUTCMinutes(), date.getUTCSeconds());
    return new Date(now_utc);
  }

  constructor() {
    var date = new Date();
    this.currentPickerDate = { isRange: false, singleDate: { jsDate: this.ToUTC(date) } };
  }

  dateChanged: EventEmitter<Date> = new EventEmitter();

  public DateChangeEvent(date: Date) {
    date = this.ToUTC(date);
    this.SetDate(date);
    this.dateChanged.emit(date);
  }
  public DateChangeEmitter() {
    return this.dateChanged;
  }

  public GetDate(): Date {
    return this.currentPickerDate.singleDate.jsDate;
  }
  public GetFullDate(): IMyDateModel {
    return this.currentPickerDate;
  }
  public SetDate(value: Date) {
    this.currentPickerDate.singleDate.jsDate = value;
  }
}
