using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace CSharpStartupTime
{
    public class StartupTimer
    {
        [DllImport("user32.dll", SetLastError = true, CharSet = CharSet.Auto)]
        public static extern int MessageBox(IntPtr hWnd, string lpText, string lpCaption, uint uType);

        private DateTime m_constructed = DateTime.Now;
        private StartupTimer()
        {
        }

        public void Print()
        {
            var str = $"{(DateTime.Now - m_constructed).TotalMilliseconds} ms";
            Debug.WriteLine(str);

            MessageBox(IntPtr.Zero, str, "Startup time: ", 0);
        }

        public static StartupTimer Instance = new();
    }
}
