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
        private DateTime m_inAppLaunch;
        private DateTime m_beforeMainWindowConstructed;
        private DateTime m_windowShown;

        private StartupTimer()
        {
        }

        public void SetAppLaunch()
        {
            m_inAppLaunch = DateTime.Now;
        }

        public void SetBeforeMainWindowConstructed()
        {
            m_beforeMainWindowConstructed = DateTime.Now;
        }

        public void SetWindowShown()
        {
            m_windowShown = DateTime.Now;
        }

        public void Print()
        {
            var cmd = Environment.GetCommandLineArgs();
            if (cmd.Length <= 1)
                return;

            var commandLaunched = DateTimeOffset.FromUnixTimeMilliseconds(long.Parse(cmd[1])).UtcDateTime.ToLocalTime();
            

            var str = $@"main(): {(m_constructed - commandLaunched).TotalMilliseconds} ms
App.OnLaunch: {(m_inAppLaunch - commandLaunched).TotalMilliseconds} ms
MainWindow constructor: {(m_beforeMainWindowConstructed - commandLaunched).TotalMilliseconds} ms
MainWindow shown: {(m_windowShown - commandLaunched).TotalMilliseconds} ms";

            Debug.WriteLine(str);
            MessageBox(IntPtr.Zero, str, "Startup time: ", 0);
        }

        public static StartupTimer Instance = new();
    }
}
