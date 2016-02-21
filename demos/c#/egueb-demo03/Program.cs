using System;
using Gtk;
using Egueb.Dom;

namespace eguebdemo03
{
	class MainClass
	{
		public static void Main (string[] args)
		{
			Egueb.Dom.Main.Init ();
			Ender.Main.Init ();
			Application.Init ();
			MainWindow win = new MainWindow ();
			win.ShowAll ();
			Application.Run ();
		}
	}
}
