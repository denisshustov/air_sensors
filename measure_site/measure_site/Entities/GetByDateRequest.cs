using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;

namespace measure_site.Entities
{
    public class GetByDateRequest
    {
        public DateTime Date { get; set; }
        public int Year { get; set; }
        public int Month { get; set; }
        public int Day { get; set; }
    }
}
