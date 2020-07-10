import { BrowserModule } from '@angular/platform-browser';
import { NgModule } from '@angular/core';
import { FormsModule } from '@angular/forms';
import { HttpClientModule, HTTP_INTERCEPTORS } from '@angular/common/http';
import { RouterModule } from '@angular/router';
import { ChartsModule } from 'ng2-charts';

import { AppComponent } from './app.component';
import { NavMenuComponent } from './nav-menu/nav-menu.component';
import { MyDatePickerModule } from 'mydatepicker';
import { ReactiveFormsModule } from '@angular/forms';
import { FirstScreenComponent } from './firstScreen/firstScreen.component';
import { SecondScreenComponent } from './secondScreen/secondScreen.component';
import { HourStaisticsComponent } from './hourStaistics/hourStaistics.component';

@NgModule({
  declarations: [
    AppComponent,
    NavMenuComponent,
    FirstScreenComponent,
    SecondScreenComponent,
    HourStaisticsComponent
  ],
  imports: [
    BrowserModule.withServerTransition({ appId: 'ng-cli-universal' }),
    HttpClientModule,    
    ChartsModule,
    MyDatePickerModule,
    FormsModule, ReactiveFormsModule,
    RouterModule.forRoot([
      { path: '', component: FirstScreenComponent, pathMatch: 'full' },
      { path: 'secondScreen', component: SecondScreenComponent, pathMatch: 'full' },
      { path: 'firsScreen', component: FirstScreenComponent, pathMatch: 'full' },
      { path: 'hourStaistics', component: HourStaisticsComponent, pathMatch: 'full' }
    ])
  ],
  providers: [],
  bootstrap: [AppComponent]
})
export class AppModule { }
