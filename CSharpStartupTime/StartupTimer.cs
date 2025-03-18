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
        private string m_arg;

        private StartupTimer()
        {
        }

        public void SetStartTime(string arg)
        {
            m_arg = arg;
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
            if (m_arg != null)
            {
                m_constructed = DateTimeOffset.FromUnixTimeMilliseconds(long.Parse(m_arg)).UtcDateTime.ToLocalTime();
            }

            var str = $@"App.OnLaunch: {(m_inAppLaunch - m_constructed).TotalMilliseconds} ms
MainWindow constructor: {(m_beforeMainWindowConstructed - m_constructed).TotalMilliseconds} ms
MainWindow shown: {(m_windowShown - m_constructed).TotalMilliseconds} ms";

            Debug.WriteLine(str);
            MessageBox(IntPtr.Zero, str, "Startup time: ", 0);
        }

        public static StartupTimer Instance = new();
    }
}
