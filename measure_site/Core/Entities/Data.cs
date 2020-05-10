using System;
using System.Data;
using System.Net.Sockets;

namespace Core
{
    public class Data
    {
        public long id { get; set; }
        public DateTime time_stamp { get; set; }
        public decimal ms5611_temp { get; set; }
        public decimal ms5611_pressue { get; set; }
        public decimal mhz19_co2 { get; set; }
        public decimal mhz19_temp { get; set; }
        public decimal ds18b20_temp { get; set; }
    }
}
