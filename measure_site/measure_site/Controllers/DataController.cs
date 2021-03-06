﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Core;
using Core.Context;
using measure_site.Entities;
using Microsoft.AspNetCore.Mvc;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;

namespace measure_site.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class DataController : Controller
    {
        private readonly ILogger<DataController> _logger;
        private readonly ApplicationContext _context;

        public DataController(ILogger<DataController> logger,
            ApplicationContext context)
        {
            _context = context;
            _logger = logger;
        }

        [HttpGet("data")]
        public async Task<IEnumerable<Data>> GetAsync()
        {
            return await _context.Data.ToListAsync();
        }

        [HttpGet("dataByDatePeriod")]
        public async Task<IEnumerable<Data>> GetByDatePeriodAsync(DateTime from, DateTime to)
        {
            return await _context.Data
                .Where(d => d.time_stamp >= from && d.time_stamp <= to)
                .OrderByDescending(x => x.time_stamp)
                .ToListAsync();
        }

        [HttpPost("dataByDate")]
        public async Task<IEnumerable<Data>> GetByDateAsync(GetByDateRequest request)
        {
            var date = request.Date.ToLocalTime();
            var result = await _context.Data
                .Where(d => d.time_stamp >= date && d.time_stamp <= date.AddHours(24))
                .OrderByDescending(x => x.time_stamp)
                .ToListAsync();
            return result;
        }
        
        [HttpGet("dataByLastHour")]
        public async Task<IEnumerable<Data>> GetDataByLastHour()
        {
            var maxDate = await _context.Data.MaxAsync(x=>x.time_stamp);
            maxDate = new DateTime(maxDate.Year, maxDate.Month, maxDate.Day, maxDate.Hour, 0, 0);
            return await _context.Data
                .Where(d => d.time_stamp >= maxDate.AddHours(-1) && d.time_stamp <= maxDate)
                .OrderByDescending(x => x.time_stamp)
                .ToListAsync();
        }

        //[HttpGet("insertTestData")]
        //public async Task<OkResult> InsertTestData(DateTime from, DateTime to)
        //{
        //    var lst = new List<Data>();
        //    var ds18b20_temp = 5M;
        //    var ds18b20_temp_forward = true;
        //    var r = new Random();
        //    for (int i = 0; i < 1000; i++)
        //    {
        //        var d = new Data()
        //        {
        //            time_stamp = DateTime.Now.AddMinutes(i),
        //            ds18b20_temp = ds18b20_temp,
        //            mhz19_co2 = ds18b20_temp * r.Next(1, 10),
        //            mhz19_temp = ds18b20_temp - r.Next(1, 5),
        //            ms5611_pressue = ds18b20_temp - r.Next(1, 55),
        //            ms5611_temp = ds18b20_temp - r.Next(1, 5)
        //        };
        //        if (ds18b20_temp_forward)
        //            ds18b20_temp += 0.5M;
        //        else
        //            ds18b20_temp -= 0.5M;
        //        if (ds18b20_temp > 30)
        //            ds18b20_temp_forward = false;
        //        if (ds18b20_temp <= 5)
        //            ds18b20_temp_forward = true;
        //        await _context.Data.AddAsync(d);
        //    }
        //    //await _context.SaveChangesAsync();

        //    return Ok();
        //}
    }
}
