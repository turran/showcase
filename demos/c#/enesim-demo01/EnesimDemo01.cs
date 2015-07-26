using System;
using Enesim;
using Ender;
using Eina;

namespace enesimdemo01
{
	public class EnesimDemo01
	{
		static bool DamageCb (Renderer r, Eina.Rectangle area, bool past, IntPtr data)
		{
			// Check downcast
			if (r is RendererCircle) {
				Console.WriteLine ("is circle");
			}

			// Check struct
			Console.WriteLine ("damage received " + area.X + " " + area.Y + " " + area.W + " " + area.H);
			return true;
		}

		public EnesimDemo01 ()
		{

		}

		static void Main() {
			// Initialize ender to support downcasting correctly
			Ender.Main.Init ();
			// Initialize enesim
			Enesim.Main.Init ();

			RendererCircle r = new RendererCircle ();
			// Test properties
			r.Radius = 100;
			r.X = 160;
			r.Y = 120;
			// Test parent properties
			r.StrokeColor = Color.ComponentsFrom (0xff, 0x00, 0xff, 0x00);
			r.StrokeWeight = 10;
			r.FillColor = Color.ComponentsFrom (0xff, 0xff, 0x00, 0x00);
			// Test enum flags
			r.DrawMode = (int)(RendererShapeDrawMode.Fill | RendererShapeDrawMode.Stroke);

			// Test callbacks/delegates
			r.GetDamages (DamageCb, IntPtr.Zero);

			// Draw it
			Log log;
			Eina.Error err;
			Surface s = new Surface (Format.Enum.Argb8888, 320, 240);
			r.Draw (s, Rop.Fill, null, 0, 0, out log);
			ImageFile.Save ("out.png", s.GetBuffer (), null, out err);
		}
	}
}

