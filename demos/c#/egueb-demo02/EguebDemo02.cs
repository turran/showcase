using System;
using System.Reflection;
using System.Runtime.InteropServices;
using System.IO;
using Egueb.Dom;
using Enesim;
using Gegueb;


namespace eguebdemo02
{
	public class EguebDemo02
	{
		static Enesim.Stream LoadResource()
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			string resourceName = "eguebdemo02.Resources.EguebDemo02.svg";
			System.IO.Stream stream = assembly.GetManifestResourceStream (resourceName);
			StreamReader reader = new StreamReader (stream);
			string result = reader.ReadToEnd();
			IntPtr rawString = Marshal.StringToHGlobalAnsi (result);
			Enesim.Stream s = new Enesim.StreamBuffer (rawString, (IntPtr)result.Length, Marshal.FreeHGlobal);
			return s;
		}

		static void Main()
		{
			Document doc;
			Application.Init ();

			Parser.Parse (LoadResource (), out doc);
			if (doc == null) {
				Console.WriteLine ("No doc");
				return;
			}

			Window win = Application.New (doc, 0, 0, -1, -1);
			Application.Run ();
			win.CloseNotify ();
		}
	}
}

