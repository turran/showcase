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
			int width = 256;
			int height = 256;
			bool needs_process = true;

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

			Egueb.Dom.FeatureWindow window = (Egueb.Dom.FeatureWindow)topmost.GetFeature (Egueb.Dom.FeatureWindow.NAME, null);
			if (window == null) {
				Console.WriteLine ("No window feature");
				return;
			} else {
				Egueb.Dom.FeatureWindowType type;
				if (window.GetType (out type))
				{
					if (type == Egueb.Dom.FeatureWindowType.Master) {
						window.SetContentSize (width, height);
					}
					window.GetContentSize (out width, out height);
					if (width <= 0 || height <= 0) {
						Console.WriteLine ("Invalid size of " + width + "x" + height);
						return;
					}
					doc.Process ();
					needs_process = false;
				}
			}

			Egueb.Dom.FeatureRender render = (Egueb.Dom.FeatureRender)topmost.GetFeature (Egueb.Dom.FeatureRender.NAME, null);
			if (render == null) {
				Console.WriteLine ("No render feature");
				return;
			}

			if (needs_process)
				doc.Process ();

			Enesim.Surface s = new Enesim.Surface (Format.Enum.Argb8888, (uint)width, (uint)height);
			render.Draw (s, Enesim.Rop.Fill, null, 0, 0, null);

			Eina.Error err;
			ImageFile.Save ("out.png", s.GetBuffer (), null, out err);
		}
	}
}