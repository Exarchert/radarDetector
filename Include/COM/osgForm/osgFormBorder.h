#pragma once
#include <osgWidget/Table>
#include <osgWidget/Label>
#include "osgButton.h"
#include "osgFormUtils.h"
using namespace osgWidget;
namespace osgForm{
class COM_EXPORT osgFormBorder : public Table
{
public:
    enum CornerType
    {
        CORNER_LOWER_LEFT,
        CORNER_LOWER_RIGHT,
        CORNER_UPPER_LEFT,
        CORNER_UPPER_RIGHT
    };

    enum BorderType
    {
        BORDER_LEFT,
        BORDER_RIGHT,
        BORDER_TOP,
        BORDER_BOTTOM
    };
	
	enum ControlBoxType
	{
		CONTROLBOX_COLOSE,
		CONTROLBOX_MINSIZE
	};
	
    enum FrameOptions
    {
        FRAME_RESIZE  = 1,
        FRAME_MOVE    = 2,
        FRAME_ALL     = FRAME_RESIZE | FRAME_MOVE
    };
	static std::string cornerTypeToString (CornerType);
    static std::string borderTypeToString (BorderType);
    static std::string controlBoxTypeToString(ControlBoxType);
	
	class ControlCallback : public ButtonCallback
	{
	public:
		ControlCallback()
		{
			_parent = NULL;
			_handler = NULL;
		}
		virtual void operator()(double x,double y,std::string ID,int type)
		{
			if(!_parent) return;
			if(type ==2)
			{
				if(ID == "ControlBoxMinSize")
				{
					_parent->minSize();
					if(_handler)
						_handler->minSizeButtonClick();
				}
				else if(ID == "ControlBoxClose")
				{
					_parent->close();
					if(_handler)
						_handler->closeButtonClick();
				}
			}
		}

		void setParent(osgFormBorder* parent){_parent = parent;}
		void setEventHandler(FormHandler* handler){_handler = handler;}
	private:
		osgFormBorder*	_parent;
		FormHandler*	_handler;
		
	};

    class  Corner: public Widget
    {
        public:

            Corner (CornerType, float w , float h ,std::string skinpath,Color color);

            virtual void parented  (Window*);
            virtual bool mouseDrag (double, double, WindowManager*);
		protected:
			CornerType _corner;
    };

    class  Border: public Widget
    {

        public:
            Border (BorderType, float w , float h ,std::string skinpath,Color color);
            //virtual void parented   (Window*);
            virtual bool mouseDrag  (double, double, WindowManager*);
            virtual bool mouseRelease(double, double, WindowManager*);
            
        protected:
            BorderType	_border;
            bool		_resize;
    };

	class Caption : public Label
	{
		public:
			Caption(float w,float h ,std::string label,std::string skinpath,Color color);
			//virtual void positioned ();
			virtual bool mouseDrag(double, double, WindowManager*);
	};
	
	class Title : public Table
	{
		public:
			Title(std::string label ,float w,float h,std::string skinpath,Color color,bool haveCtrBox);
			~Title();
			virtual void parented   (Window*);
			Caption* getCaption();
			void setControlCallback(ControlCallback* callback);

		private:
			osg::ref_ptr<osgButton>					_minButton;
			osg::ref_ptr<osgButton>					_closeButton;
			osg::ref_ptr<Caption>					_caption;
	};

	
public:
	//Form(const std::string& name, unsigned int flags);
	
	osgFormBorder(const std::string& title, float w, float h,const std::string& skinPath, Color color,unsigned int flags =0,bool haveCtrlBox = true);
	~osgFormBorder(void);
    
	void setCaptionLabel(std::string);
	void setCaptionFont(std::string);
	void setCaptionFontSize(int);
	void setCaptionColor(Color);
	void close();
	bool minSize();
	EmbeddedWindow* getWorkSpace();
protected:
	

    bool canResize() const
    {
        return (_flags & FRAME_RESIZE) != 0;
    }

    bool canMove() const
    {
        return (_flags & FRAME_MOVE) != 0;
    }
	
	float getCurWorkspaceH(){return getHeight() - _th-_bbw;}
	float getCurWorkspaceW(){return getWidth() -_lbw - _rbw;}
	void setMinWH(float,float);
	virtual void dragBottom();
private:
	void wResize();
	void hResize();
	void setItemHide();
    void setItemUnHide();
protected:
	unsigned int									_flags;
	unsigned int									_flagsBak;
	osg::ref_ptr<Title>								_title;
	osg::ref_ptr<Window>							_windows;
	osg::ref_ptr<ControlCallback>					_callback;
	float											_h;
	float											_w;
	std::string										_skinpath;
	Color											_color;	
	bool											_isMin;	
	float											_beforW;
	float											_beforH;
	XYCoord											_pos;	
	float											_th;
	float											_lbw;
	float											_rbw;
	float											_bbw;	
	float											_minW;
	float											_minH;	
	//ButtonCallback*									_callback;
							
};
}