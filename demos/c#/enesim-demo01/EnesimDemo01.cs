using System;
using Enesim;
using Eina;

namespace enesimdemo01
{
	public class EnesimDemo01
	{
		static bool DamageCb (Enesim.Renderer r2, Eina.Rectangle area, bool past, IntPtr data)
		{
			Console.WriteLine ("damage received " + area.X + " " + area.Y + " " + area.W + " " + area.H);
			return true;
		}

		public EnesimDemo01 ()
		{

		}

		static void Main() {
			Enesim.Main.Init ();
			Enesim.Renderer.Circle r = new Enesim.Renderer.Circle ();
			r.Radius = 100;
			r.GetDamages (DamageCb, IntPtr.Zero);
		}
	}
}

