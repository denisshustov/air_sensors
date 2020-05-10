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
    [Route("[controller]")]
    public class DataController : ControllerBase
    {
        private readonly ILogger<DataController> _logger;
        private readonly ApplicationContext _context;

        public DataController(ILogger<DataController> logger,
            ApplicationContext context)
        {
            _context = context;
            _logger = logger;
        }

        [HttpGet]
        public async Task<IEnumerable<Data>> GetAsync()
        {
            return await _context.Data.ToListAsync();
        }

        [HttpGet]
        public async Task<IEnumerable<Data>> GetByDateAsync(DateTime from, DateTime to)
        {
            //from = new DateTime(from.Year, from.Month, from.Day);
            return await _context.Data
                .Where(d => d.time_stamp > from && d.time_stamp < to)
                .ToListAsync();
        }
    }
}
