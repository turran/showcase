using System;
using System.Reflection;
using System.IO;

using Egueb;
using Ender;
using Enesim;
using Eina;
using System.Runtime.InteropServices;

namespace eguebdemo01
{
	public class EguebDemo01
	{
		static Enesim.Stream LoadResource()
		{
			Assembly assembly = Assembly.GetExecutingAssembly();
			string resourceName = "eguebdemo01.Resources.EguebDemo01.svg";
			System.IO.Stream stream = assembly.GetManifestResourceStream (resourceName);
			StreamReader reader = new StreamReader (stream);
			string result = reader.ReadToEnd();
			IntPtr rawString = Marshal.StringToHGlobalAnsi (result);
			Enesim.Stream s = new Enesim.StreamBuffer (rawString, (IntPtr)result.Length, Marshal.FreeHGlobal);
			return s;
		}

		static void Main()
		{
			Egueb.Dom.Main.Init ();
			Ender.Main.Init ();

			Egueb.Dom.Document doc;

			//doc.AppendChild (svg, new Eina.Error());
			// TODO handle correctly the postunmanaged on out objects
			Egueb.Dom.Parser.Parse (LoadResource (), out doc);
			if (doc == null) {
				Console.WriteLine ("No doc");
				return;
			}

			Egueb.Dom.Element topmost = doc.DocumentElement;
			if (topmost == null) {
				Console.WriteLine ("No topmost");
				return;
			}

			//topmost.GetFeature (null, 0);
			Console.WriteLine ("node is " + doc);
			Enesim.Surface s = new Enesim.Surface (Format.Enum.Argb8888, 320, 240);
			//Egueb.Svg.Draw (s, Enesim.Rop.Fill, null, 0, 0, null);

			Egueb.Svg.ColorAnimated ca = new Egueb.Svg.ColorAnimated ();
			ca.Anim.B = 0xff;
			Console.WriteLine ("color " + ca.Anim.B);
		}
	}
}

