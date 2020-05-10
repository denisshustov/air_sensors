using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Core;
using Core.Context;
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

        [HttpGet("dataByDate")]
        public async Task<IEnumerable<Data>> GetByDateAsync(DateTime from, DateTime to)
        {
            return await _context.Data
                .Where(d => d.time_stamp > from && d.time_stamp < to)
                .ToListAsync();
        }

        [HttpGet("insertTestData")]
        public async Task InsertTestData(DateTime from, DateTime to)
        {
            var lst = new List<Data>();
            for (int i = 0; i < 1000; i++)
            {
                lst.Add(new Data()
                {
                    time_stamp = DateTime.Now,

                });
            }
            //await _context.Data.AddAsync(new Data() { 
            //});
        }
    }
}
