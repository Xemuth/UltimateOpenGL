//#ifndef _UltimateOpenGL_UOGLCtrl_h_
//#define _UltimateOpenGL_UOGLCtrl_h_
/**
	UltimateOpenGL Ctrl is an Ultimate++ Ctrl designed for allow Ultimate OpenGL
	to work without mutch knowledge arround OpenGL initialisation...
**/
/*
#include <CtrlCore/CtrlCore.h>

#if defined(GUI_X11) || defined(GUI_GTK)
#define Time    XTime
#define Font    XFont
#define Display XDisplay
#define Picture XPicture
#define Status  int
#endif

#include <plugin/glew/glew.h>

#ifdef PLATFORM_WIN32
#include <plugin/glew/wglew.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#if defined(GUI_X11) || defined(GUI_GTK)

#include <GL/glx.h>

#undef  Status
#undef  Picture
#undef  Time
#undef  Font
#undef  Display
#endif


class UOGLCtrl : public Upp::Ctrl {
	typedef UOGLCtrl CLASSNAME;
	
	private:
		bool Context_Initialized = false;
		UltimateOpenGL_Context context;
		
		bool OpenGL_Initialized = false;
		void InitOpenGL(); //It will init openGL to the current thread, without initialisation, it can't show context
	public:
		UOGLCtrl(); //UOGLCtrl constructor should only
		
		bool IsContextInitialized();
		bool IsOpenGLInitialized();
		UltimateOpenGL_Context& GetContext();
		
		virtual void Init();
		virtual void Paint();
		virtual void Shutdown();
		
		//Resize event
		virtual void Layout();
		virtual void Resize(int w, int h);
		
		//Mouse Event :
		//Left click :
		virtual void LeftHold(Point p,dword keyflags);
		virtual void LeftDown(Point p,dword keyflags);
		virtual void LeftUp(Point p,dword keyflags);
		virtual void LeftDrag(Point p,dword keyflags);
		virtual void LeftDouble(Point p,dword keyflags);
		virtual void LeftTriple(Point p,dword keyflags);
		//Right click :
		virtual void RightHold(Point p,dword keyflags);
		virtual void RightDown(Point p,dword keyflags);
		virtual void RightUp(Point p,dword keyflags);
		virtual void RightDrag(Point p,dword keyflags);
		virtual void RightDouble(Point p,dword keyflags);
		virtual void RightTriple(Point p,dword keyflags);
		//Wheel click:
		virtual void MiddleHold(Point p,dword keyflags);
		virtual void MiddleDown(Point p,dword keyflags);
		virtual void MiddleUp(Point p,dword keyflags);
		virtual void MiddleDrag(Point p,dword keyflags);
		virtual void MiddleDouble(Point p,dword keyflags);
		virtual void MiddleTriple(Point p,dword keyflags);
		//Wheel event :
		virtual void MouseWheel(Point p, int zDelta,dword Keyflags);
		//Event :
		virtual void MouseEnter(Point p,dword keyflags);
		virtual void MouseLeave();
		virtual void MouseMove(Point p,dword keyflags);
		//KeyBoard Event :
		virtual bool Key(dword key, int count);
};
*/
//#endif
