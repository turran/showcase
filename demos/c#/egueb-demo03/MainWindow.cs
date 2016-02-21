using System;
using Gtk;
using Egueb.Dom;
using Gegueb;
using Enesim;
using System.Reflection;
using System.Runtime.InteropServices;
using System.IO;

public partial class MainWindow: Gtk.Window
{
	Enesim.Stream LoadResource()
	{
		Assembly assembly = Assembly.GetExecutingAssembly();
		string resourceName = "eguebdemo03.Resources.EguebDemo03.svg";
		System.IO.Stream stream = assembly.GetManifestResourceStream (resourceName);
		StreamReader reader = new StreamReader (stream);
		string result = reader.ReadToEnd();
		IntPtr rawString = Marshal.StringToHGlobalAnsi (result);
		Enesim.Stream s = new Enesim.StreamBuffer (rawString, (IntPtr)result.Length, Marshal.FreeHGlobal);
		return s;
	}

	public MainWindow () : base (Gtk.WindowType.Toplevel)
	{
		Document doc = null;

		Build ();

		Gegueb.Widget w = new Gegueb.Widget ();
		Parser.Parse (LoadResource (), out doc);
		w.Document = doc;
		vbox1.Add (w);
	}

	protected void OnDeleteEvent (object sender, DeleteEventArgs a)
	{
		Gtk.Application.Quit ();
		a.RetVal = true;
	}
}
