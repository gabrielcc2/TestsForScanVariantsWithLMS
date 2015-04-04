package scala.virtualization.lms
package common

import java.io.PrintWriter
import scala.virtualization.lms.internal._
import scala.reflect.SourceContext

trait MiscOps extends Base {
  /**
   * Other things that need to get lifted like exit, there should be
   * a better way to do this
   */

  def print(x: Rep[Any])(implicit pos: SourceContext): Rep[Unit]
  def println(x: Rep[Any])(implicit pos: SourceContext): Rep[Unit]
  def printf(f: String, x: Rep[Any]*)(implicit pos: SourceContext): Rep[Unit]
  // TODO: there is no way to override this behavior
  def exit(status: Int)(implicit pos: SourceContext): Rep[Nothing] = exit(unit(status))
  def exit()(implicit pos: SourceContext): Rep[Nothing] = exit(0)
  def exit(status: Rep[Int])(implicit pos: SourceContext): Rep[Nothing]
  def error(s: Rep[String])(implicit pos: SourceContext): Rep[Nothing]
  def returnL(x: Rep[Any])(implicit pos: SourceContext): Rep[Unit]
  
    /*Introduced code, to be used to support vectorization*/
		 def sizeof(x: String)(implicit pos: SourceContext): Rep[Int]
		 def referenceOperator(x: Rep[Float])(implicit pos: SourceContext): Rep[Array[Float]]
  		 def intptr_t(x: Rep[Array[Float]])(implicit pos: SourceContext): Rep[Int]
  		 def _mm_set1_ps(x: Rep[Float])(implicit pos: SourceContext): Rep[Int]
  		 def _mm_load_ps(x: Rep[Array[Float]])(implicit pos: SourceContext): Rep[Int]
  		 def _mm_movemask_ps (x:Rep[Int])(implicit pos: SourceContext): Rep[Int]
  		 def bitwiseCompare(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]
  		 def binaryRightShift(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]
         def _mm_cmpeq_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]
  	     def _mm_cmpgt_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]
    	 def _mm_cmpge_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]
         def _mm_cmplt_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]
    	 def _mm_cmple_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]
         def _mm_cmpneq_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]
}

trait MiscOpsExp extends MiscOps with EffectExp {
  case class Print(x: Exp[Any]) extends Def[Unit]
  case class PrintLn(x: Exp[Any]) extends Def[Unit]
  case class PrintF(f: String, x: List[Exp[Any]]) extends Def[Unit]  
  case class Exit(s: Exp[Int]) extends Def[Nothing]
  case class Error(s: Exp[String]) extends Def[Nothing]
  case class Return(x: Exp[Any]) extends Def[Unit]
  
  /*Introduced code, to be used to support vectorization*/
  		case class SizeOf(x: String) extends Def[Int]
  		case class ReferenceOf(x: Rep[Float]) extends Def[Array[Float]]
  		case class IntPtr(x: Rep[Array[Float]]) extends Def[Int]
  		case class MMSet1PS(x: Rep[Float]) extends Def[Int]
  		case class MMLoadPS(x: Rep[Array[Float]]) extends Def[Int]
        case class MMMovemaskPS(x: Rep[Int]) extends Def[Int]
    	case class BitwiseCompare(x: Rep[Int], y: Rep[Int]) extends Def[Int]
    	case class BinaryRightShift(x: Rep[Int], y: Rep[Int]) extends Def[Int]
        case class MMCmpeqPS(x: Rep[Int], y: Rep[Int]) extends Def[Int]
        case class MMCmpgtPS(x: Rep[Int], y: Rep[Int]) extends Def[Int]
        case class MMCmpgePS(x: Rep[Int], y: Rep[Int]) extends Def[Int]
        case class MMCmpltPS(x: Rep[Int], y: Rep[Int]) extends Def[Int]
        case class MMCmplePS(x: Rep[Int], y: Rep[Int]) extends Def[Int]
        case class MMCmpneqPS(x: Rep[Int], y: Rep[Int]) extends Def[Int]
  
  
  def print(x: Exp[Any])(implicit pos: SourceContext) = reflectEffect(Print(x)) // TODO: simple effect
  def println(x: Exp[Any])(implicit pos: SourceContext) = reflectEffect(PrintLn(x)) // TODO: simple effect
  def printf(f: String, x: Rep[Any]*)(implicit pos: SourceContext): Rep[Unit] = reflectEffect(PrintF(f, x.toList))
  def exit(s: Exp[Int])(implicit pos: SourceContext) = reflectEffect(Exit(s))
  def error(s: Exp[String])(implicit pos: SourceContext) = reflectEffect(Error(s))
  def returnL(x: Exp[Any])(implicit pos: SourceContext) = {
    printlog("warning: staged return statements are unlikely to work because the surrounding source method does not exist in the generated code.")
    printsrc(raw"in ${quotePos(x)}")
    reflectEffect(Return(x))
  }
  
  /*Introduced code, to be used to support vectorization*/  
  		def sizeof(x: String)(implicit pos: SourceContext): Rep[Int]=reflectEffect(SizeOf(x))
  		def referenceOperator(x: Rep[Float])(implicit pos: SourceContext): Rep[Array[Float]]=reflectEffect(ReferenceOf(x))
  		def intptr_t(x: Rep[Array[Float]])(implicit pos: SourceContext): Rep[Int]=reflectEffect(IntPtr(x))
  		def _mm_set1_ps (x:Rep[Float])(implicit pos: SourceContext): Rep[Int]=reflectEffect(MMSet1PS(x))
  		def _mm_load_ps (x:Rep[Array[Float]])(implicit pos: SourceContext): Rep[Int]=reflectEffect(MMLoadPS(x))
  		def _mm_movemask_ps (x:Rep[Int])(implicit pos: SourceContext): Rep[Int]= reflectEffect(MMMovemaskPS(x))
  		def bitwiseCompare(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]= reflectEffect(BitwiseCompare(x,y))
  		def binaryRightShift(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]=reflectEffect(BinaryRightShift(x,y))
  		def _mm_cmpeq_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]=reflectEffect(MMCmpeqPS(x, y))
  		def _mm_cmpgt_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]=reflectEffect(MMCmpgtPS(x, y))
  		def _mm_cmpge_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]=reflectEffect(MMCmpgePS(x, y))
  		def _mm_cmplt_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]=reflectEffect(MMCmpltPS(x, y))
  		def _mm_cmple_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]=reflectEffect(MMCmplePS(x, y))
        def _mm_cmpneq_ps(x:Rep[Int], y:Rep[Int])(implicit pos: SourceContext): Rep[Int]=reflectEffect(MMCmpneqPS(x, y))  		
  				
 override def mirror[A:Manifest](e: Def[A], f: Transformer)(implicit pos: SourceContext): Exp[A] = (e match {
    case Reflect(Error(x), u, es) => reflectMirrored(Reflect(Error(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
    case Reflect(Print(x), u, es) => reflectMirrored(Reflect(Print(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
    case Reflect(PrintLn(x), u, es) => reflectMirrored(Reflect(PrintLn(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
    case Reflect(PrintF(fm,x), u, es) => reflectMirrored(Reflect(PrintF(fm,f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
    case Reflect(Exit(x), u, es) => reflectMirrored(Reflect(Exit(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
    case Reflect(Return(x), u, es) => reflectMirrored(Reflect(Return(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
    
    /*Introduced code, to be used to support vectorization*/
    /*TODO Evaluate if reflection is needed. If so, quotations and Const() can be removed by post-processing.*/
 //   	case Reflect(SizeOf(x), u, es) => reflectMirrored(Reflect(SizeOf(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //   	case Reflect(ReferenceOf(x), u, es) => reflectMirrored(Reflect(ReferenceOf(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //   	case Reflect(IntPtr(x), u, es) => reflectMirrored(Reflect(IntPtr(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //     case Reflect(MMSet1PS(x), u, es) => reflectMirrored(Reflect(MMSet1PS(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //     case Reflect(MMLoadPS(x), u, es) => reflectMirrored(Reflect(MMLoadPS(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //     case Reflect(MMMovemaskPS(x), u, es) => reflectMirrored(Reflect(MMMovemaskPS(f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)    
 //     case Reflect(BitwiseCompare(fm, x), u, es) => reflectMirrored(Reflect(BitwiseCompare(fm, f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
//      case Reflect(BinaryRightShift(fm, x), u, es) => reflectMirrored(Reflect(BinaryRightShift(fm, f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos) 
//     case Reflect(MMCmpeqPS(fm, x), u, es) => reflectMirrored(Reflect(MMCmpeqPS(fm, f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
//     case Reflect(MMCmpgtPS(fm, x), u, es) => reflectMirrored(Reflect(MMCmpgtPS(fm, f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //     case Reflect(MMCmpgePS(fm, x), u, es) => reflectMirrored(Reflect(MMCmpgePS(fm, f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //     case Reflect(MMCmpltPS(fm, x), u, es) => reflectMirrored(Reflect(MMCmpltPS(fm, f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //     case Reflect(MMCmplePS(fm, x), u, es) => reflectMirrored(Reflect(MMCmplePS(fm, f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)
 //     case Reflect(MMCmpneqPS(fm, x), u, es) => reflectMirrored(Reflect(MMCmpneqPS(fm, f(x)), mapOver(f,u), f(es)))(mtype(manifest[A]), pos)  
    
    case _ => super.mirror(e,f)
  }).asInstanceOf[Exp[A]]
}

trait ScalaGenMiscOps extends ScalaGenEffect {
  val IR: MiscOpsExp
  import IR._

  override def emitNode(sym: Sym[Any], rhs: Def[Any]) = rhs match {
    case PrintF(f,xs) => emitValDef(sym, src"printf(${f::xs})")
    case PrintLn(s) => emitValDef(sym, src"println($s)")
    case Print(s) => emitValDef(sym, src"print($s)")
    case Exit(a) => emitValDef(sym, src"exit($a)")
    case Return(x) => emitValDef(sym, src"return $x")
    case Error(s) => emitValDef(sym, src"error($s)")
    
    /*Introduced code, to be used to support vectorization*/
    		case SizeOf(s)=>emitValDef(sym, src"sizeof($s)")
    		case ReferenceOf(s)=>emitValDef(sym, src"&($s)")
    		case IntPtr(s)=>emitValDef(sym, src"(intptr_t)($s)")
    		case MMSet1PS(s)=>emitValDef(sym,src"_mm_set1_ps($s)")
    		case MMLoadPS(s)=>emitValDef(sym,src"_mm_load_ps($s)")
    		case MMMovemaskPS(s)=>emitValDef(sym,src"_mm_movemask_ps($s)")
    		case BitwiseCompare(f,xs)=>emitValDef(sym,src"($f & $xs)")
    		case BinaryRightShift(f,xs)=>emitValDef(sym,src"($f >> $xs)")
    		case MMCmpeqPS(f,xs)=>emitValDef(sym,src"_mm_cmpeq_ps($f,$xs)")
     		case MMCmpgtPS(f,xs)=>emitValDef(sym,src"_mm_cmpgt_ps($f,$xs)")
      		case MMCmpgePS(f,xs)=>emitValDef(sym,src"_mm_cmpge_ps($f,$xs)")
      		case MMCmpltPS(f,xs)=>emitValDef(sym,src"_mm_cmplt_ps($f,$xs)")
      		case MMCmplePS(f,xs)=>emitValDef(sym,src"_mm_cmple_ps($f,$xs)")
    		case MMCmpneqPS(f,xs)=>emitValDef(sym,src"_mm_cmpneq_ps($f,$xs)")
     		
    case _ => super.emitNode(sym, rhs)
  }
}


trait CGenMiscOps extends CGenEffect {
  val IR: MiscOpsExp
  import IR._

  def format(s: Exp[Any]): String = {
    remap(s.tp) match {
      case "uint16_t" => "%c"
      case "bool" | "int8_t" | "int16_t" | "int32_t" => "%d"
      case "int64_t" => "%ld"
      case "float" | "double" => "%f"
      case "string" => "%s" 
      case _ => throw new GenerationFailedException("CGenMiscOps: cannot print type " + remap(s.tp))
    }
  }

  def quoteRawString(s: Exp[Any]): String = {
    remap(s.tp) match {
      case "string" => quote(s) + ".c_str()"
      case _ => quote(s)
    }
  }

  override def emitNode(sym: Sym[Any], rhs: Def[Any]) = rhs match {
    case PrintF(f,x) => stream.println("printf(" + ((Const(f:String)::x).map(quoteRawString)).mkString(",") + ");")
    case PrintLn(s) => stream.println("printf(\"" + format(s) + "\\n\"," + quoteRawString(s) + ");")
    case Print(s) => stream.println("printf(\"" + format(s) + "\"," + quoteRawString(s) + ");")
    case Exit(a) => stream.println("exit(" + quote(a) + ");")
    case Return(x) => stream.println("return " + quote(x) + ";")
    case Error(s) => stream.println("error(-1,0,\"%s\"," + quote(s) + ");")
    
    /*Introduced code, to be used to support vectorization*/
    		case SizeOf(s)=>emitValDef(sym, src"sizeof($s)")
    		case ReferenceOf(s)=>emitValDef(sym, src"&($s)")
    		case IntPtr(s)=>emitValDef(sym, src"(intptr_t)($s)")
    		case MMSet1PS(s)=>emitValDef(sym, src"_mm_set1_ps($s)")
    		case MMLoadPS(s)=>emitValDef(sym,src"_mm_load_ps($s)")
    		case MMMovemaskPS(s)=>emitValDef(sym,src"_mm_movemask_ps($s)")
    		case BitwiseCompare(f,xs)=>emitValDef(sym,src"($f & $xs)")
    		case BinaryRightShift(f,xs)=>emitValDef(sym,src"($f >> $xs)")
    		case MMCmpeqPS(f,xs)=>emitValDef(sym,src"_mm_cmpeq_ps($f,$xs)")
     		case MMCmpgtPS(f,xs)=>emitValDef(sym,src"_mm_cmpgt_ps($f,$xs)")
      		case MMCmpgePS(f,xs)=>emitValDef(sym,src"_mm_cmpge_ps($f,$xs)")
      		case MMCmpltPS(f,xs)=>emitValDef(sym,src"_mm_cmplt_ps($f,$xs)")
      		case MMCmplePS(f,xs)=>emitValDef(sym,src"_mm_cmple_ps($f,$xs)")
    		case MMCmpneqPS(f,xs)=>emitValDef(sym,src"_mm_cmpneq_ps($f,$xs)")
    		
    case _ => super.emitNode(sym, rhs)
  }
}

trait CudaGenMiscOps extends CudaGenEffect {
  val IR: MiscOpsExp
  import IR._

  override def emitNode(sym: Sym[Any], rhs: Def[Any]) = rhs match {
    case _ => super.emitNode(sym, rhs)
  }
}


trait OpenCLGenMiscOps extends OpenCLGenEffect {
  val IR: MiscOpsExp
  import IR._

  override def emitNode(sym: Sym[Any], rhs: Def[Any]) = rhs match {
    case _ => super.emitNode(sym, rhs)
  }
}