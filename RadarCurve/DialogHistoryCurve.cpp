// DialogCurCurve.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Resource.h"
#include "RadarCurve.h"
#include "DialogHistoryCurve.h"
#include <sstream>
#include <RadarCurve/RadarManager.h>
#include "DataTemplateTab.h"
#include "VideoTemplateTab.h"
#include "DataCountTab.h"
#include <Kernel/KernelExport.h>
#include <osg/ref_ptr>
#include "SGYWriter.h"
#include <math.h>
#include "ProcessDlg.h"
#include <osgAPEx/CoordinateTool>
#include <osg/StateAttribute>
#include <osg/LineWidth>
#include <osg/Geode>

#include <Object/CmdCenter/CmdCenter.h>
#include "VideoTemplateTab.h"
#include <shlwapi.h>
#include <osgApex/CoordinateTool>
#include <osg/StateAttribute>
#include <osg/LineWidth>
#include <osg/Geode>
#include <Kernel/KernelExport.h>

#include <Com/Command/Cmd.h>

//C++
#include <sstream>
#include <io.h>
#include <fcntl.h>
#include <iomanip>
using namespace std;

class HistoryLineCmd : public COM::CCmdObj
{
public:
	enum Oper_Type
	{
		T_ADD,
		T_DEL,
	};
	HistoryLineCmd( Oper_Type t, osg::Node *lpNode )
	{
		_t = t;
		_lpNode = lpNode;
	}
	virtual void DoAction()
	{
		if ( !_lpNode.valid() )
		{
			return;
		}

		if ( _t == T_ADD )
		{
			if ( Kernel::GetpKC()->GetTempGroupRoot()->containsNode( _lpNode ) )
			{
				return;
			}
			Kernel::GetpKC()->GetTempGroupRoot()->addChild( _lpNode );
		}else
		{
			if ( Kernel::GetpKC()->GetTempGroupRoot()->containsNode( _lpNode ) )
			{
				Kernel::GetpKC()->GetTempGroupRoot()->removeChild( _lpNode );
			}
		}

	}
	osg::ref_ptr<osg::Node> _lpNode;
	Oper_Type _t;
};

// CDialogHistoryCurve �Ի���

IMPLEMENT_DYNAMIC(CDialogHistoryCurve, CDialog)

CDialogHistoryCurve::CDialogHistoryCurve( std::string const& projectName, int channelIndex, std::string const& xmlParam, float startLen, CWnd* pParent /*=NULL*/)
	: CDialog( true, pParent)
{
	_projectName = projectName;
	_xmlParam = xmlParam;
	_channelIndex = channelIndex;

	_startPos = 0;
	_endPos = 0;

	_startLen = startLen;
	_loadProcess = NULL;
    _skip = 1;
	m_iCount = 0;
// 	_lpCtrlDialog = NULL;
// 	_lpThread = NULL;
}

CDialogHistoryCurve::~CDialogHistoryCurve()
{
// 	if ( _lpCtrlDialog )
// 	{
// 		if ( _lpCtrlDialog->GetSafeHwnd() )
// 		{
// 
// 			_lpCtrlDialog->DestroyWindow();
// 		}
// 		delete _lpCtrlDialog;
// 	}
}

void CDialogHistoryCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_HISTORY_CURVE, _radarWnd);
	DDX_Text( pDX, IDC_EDIT_START, _startPos );
	DDX_Text( pDX, IDC_EDIT_END, _endPos );
	DDX_Control( pDX, IDC_EDIT_SELECT_POS, _curLenEdit );
}


BEGIN_MESSAGE_MAP(CDialogHistoryCurve, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogHistoryCurve::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogHistoryCurve::OnBnClickedCancel)
	ON_COMMAND( ID_SHOW_VIDEO, &CDialogHistoryCurve::OnShowVideo)
	ON_COMMAND( ID_SHOW_POS, &CDialogHistoryCurve::OnShowPos)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_BUTTON_FIND, &CDialogHistoryCurve::OnBnClickedButtonFind)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, &CDialogHistoryCurve::OnBnClickedButtonExport)
	ON_MESSAGE(WM_CLICK_ON_SECTION, &CDialogHistoryCurve::OnClickCurve)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDialogHistoryCurve ��Ϣ�������

BOOL CDialogHistoryCurve::OnInitDialog()
{
    _skip = 1;
	CDialog::OnInitDialog();

	
	CString title;
	RadarManager *lpProject = RadarManager::Instance();
	if ( lpProject )
	{

		USES_CONVERSION;
		CString titleName;
		CString name;
		name = A2W( lpProject->getChannelName( _channelIndex ).c_str() );
		//titleName.Format(L"ͨ��%d-%s", index, name);
		title.Format(L"��ʷ����-ͨ��%d-%s", _channelIndex + 1, name );
	}else
	{
		title.Format(L"��ʷ����-ͨ��%d", _channelIndex + 1 );
	}

	ConfigureSet set;
	set.fromGroupXML(_xmlParam);
	int iSmpNum = atoi ( set.get("radar", "sample").c_str());
	int iSampRate = atoi ( set.get("radar", "sampleratio" ).c_str() );

	_radarWnd.SetSampleCount( RadarManager::Instance()->getSampCount( iSmpNum ) );
	_radarWnd.SetSampleRatio( RadarManager::Instance()->getSampRatio( atoi (set.get("radar", "sampleratio" ).c_str()),_channelIndex) );


	_radarWnd.SetParent( this );
	SetWindowText(title);

	_curLenEdit.SetWindowText(L"0");


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


bool CDialogHistoryCurve::init()
{

	if ( !RadarManager::Instance()->dbOpen() )
	{
		return false;
	}

	DataTemplateTab tab;
	tab.setProjectName( _projectName );

	int recordCount = 0;
	{
		DataCountTab countTab;
		countTab.setTabName( tab.getProjectName() );
		std::stringstream ss;
		ss << countTab.getSelectSql();
		if ( _startPos == _endPos )
		{
			ss << " where ChannelIndex=" << _channelIndex;
		}else
		{
			ss << " where ChannelIndex=" << _channelIndex;

			if ( _startPos - _startLen > 0 )
			{
				ss << " and LEN>=" << _startPos;
			}

			if ( _endPos - _startLen > 0 )
			{
				ss << " and LEN<=" << _endPos;
			}
		}

		recordCount = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), ss.str());

	}


	std::stringstream ss;
	if ( _startPos == _endPos )
	{
		ss << tab.getSelectSql() << " where ChannelIndex=" << _channelIndex;
	}else
	{
		ss << tab.getSelectSql() << " where ChannelIndex=" << _channelIndex;

		if ( _startPos - _startLen > 0 )
		{
			ss << " and LEN>=" << _startPos;
		}

		if ( _endPos - _startLen > 0 )
		{
			ss << " and LEN<=" << _endPos;
		}
	}

	ss << " order by id";

	if ( !tab.open( RadarManager::Instance()->getDBHelp(), ss.str() ) )
	{
		MessageBox(L"�򿪹��̱��ʧ��");
		return false;
	}


	int width = recordCount;
	int height = 0;

	if ( !_loadProcess )
	{
		_loadProcess = new CProcessDlg;
		_loadProcess->Create( CProcessDlg::IDD );
	}

	CProcessDlg *lpDlg = (CProcessDlg*)(_loadProcess);


	CDBTable::RowVector rv;

	int readCount = 0;
	unsigned int *buff = NULL;

	if ( _lpLineNode.valid() )
	{
		COM::CCmdObjQueue *cmdQueue = NULL;
		if (Kernel::GetpKC() && (cmdQueue = Kernel::GetpKC()->m_CmdCenter.GetCmdObjList()))
		{
			osg::ref_ptr<HistoryLineCmd> pCmd = new HistoryLineCmd(HistoryLineCmd::T_DEL, _lpLineNode.get());
			cmdQueue->EnterQueue(pCmd);
		}
	}

	osg::MatrixTransform *lpRootMatrix = new osg::MatrixTransform;
	osg::Geode *lpGeode = new osg::Geode;
	lpRootMatrix->addChild( lpGeode );

	osg::Geometry *lpLineGeometry = new osg::Geometry;
	lpGeode->addDrawable( lpLineGeometry );

	osg::Vec3Array *lpVertexArray = new osg::Vec3Array;
	lpLineGeometry->setVertexArray( lpVertexArray );

	osg::Vec4Array *lpColorArray = new osg::Vec4Array;
	lpColorArray->push_back( osg::Vec4( 1.0, 1.0, 0.0, 1.0 ) );
	lpLineGeometry->setColorBinding( osg::Geometry::BIND_OVERALL );
	lpLineGeometry->setColorArray( lpColorArray );
	osg::LineWidth *lw = new osg::LineWidth;
	lw->setWidth( 2.0f );
	lpLineGeometry->getOrCreateStateSet()->setAttributeAndModes( lw, osg::StateAttribute::ON );

	osg::Matrix localToWorld;
	osg::Matrix worldToLocal;

	int nScreenWidth, nScreenHeight;  
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 30;  
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	int baseWidth = nScreenWidth;

	int skip = width / nScreenWidth;
	if ( skip == 0 )
	{
		skip = 1;

		baseWidth = width;
	}else
	{
		baseWidth += width % nScreenWidth;
	}
    _skip = skip;
	lpDlg->StartProcess( baseWidth );
	_indexVector.resize( baseWidth );
	lpVertexArray->resize( baseWidth );

	while( ( rv = tab.read( 1000 ) ).size() != 0 )
	{

		if ( !buff )
		{
			RadarData *lpRD = dynamic_cast<RadarData*>( rv[0].get());
			height = lpRD->getDataCount();
			buff = new unsigned int[baseWidth * height];

			osg::Matrixd localToWorld;
			osgAPEx::ComputeLocalToWorldTransformFromLongLatHeight( lpRD->getPosition().x(), lpRD->getPosition().y(), lpRD->getPosition().z(), localToWorld );
			lpRootMatrix->setMatrix( localToWorld );

			worldToLocal = osg::Matrix::inverse( localToWorld );
		}

		for ( unsigned int i = 0; i < rv.size(); i ++ )
		{
			RadarData *lpRD = dynamic_cast<RadarData*>( rv[i].get());
			if ( !lpRD )
			{
				continue;
			}
			if ( ( ( i + readCount ) % skip ) == 0 )
			{
				int index = ( i + readCount ) / skip;
				_indexVector[index] = lpRD->getID();
				int sampCount = lpRD->getDataCount();
				int xpos = index;

				osg::Vec3d xyz;
				osgAPEx::ConvertLongLatHeightToXYZ( lpRD->getPosition().x(), lpRD->getPosition().y(), lpRD->getPosition().z(), xyz );
				osg::Vec3d localPos = xyz * worldToLocal;
				lpVertexArray->at(index) = localPos;

				for ( int j = 0; j < lpRD->getDataCount(); j ++ )
				{
					int ypos = j;

					short v = lpRD->getIndexData( j, NULL );
					COLORREF c;
					if ( lpRD->getMark() )
					{
						c = RGB ( 255, 255, 255 );				
					}else
					{
						c = _radarWnd.getColor( v );
					}

					buff[ypos * baseWidth + xpos] = c;

				}
				lpDlg->Step( 1 );
			}
		}
		readCount += rv.size();
		if ( rv.size() < 1000 )
		{
			break;
		}
	}

	_radarWnd.setRadarData( buff, baseWidth, height );
	delete []buff;

	_loadProcess->ShowWindow( SW_HIDE );

	Sleep(100);
	MSG message;
	if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))   
	{   
		if (message.hwnd == _loadProcess->m_hWnd)
		{
			::TranslateMessage(&message);   
			::DispatchMessage(&message);   
		}
	}
	_loadProcess->ShowWindow( SW_HIDE );

	lpLineGeometry->addPrimitiveSet( new osg::DrawArrays ( osg::PrimitiveSet::LINE_STRIP, 0, lpVertexArray->size() ));

	_lpLineNode = lpRootMatrix;
	if ( _lpLineNode.valid() )
	{
		COM::CCmdObjQueue *cmdQueue = NULL;
		if (Kernel::GetpKC() && (cmdQueue = Kernel::GetpKC()->m_CmdCenter.GetCmdObjList()))
		{
			osg::ref_ptr<HistoryLineCmd> pCmd = new HistoryLineCmd(HistoryLineCmd::T_ADD, _lpLineNode.get());
			cmdQueue->EnterQueue(pCmd);
		}
	}

	return true;
}




bool CDialogHistoryCurve::clear()
{

	return true;
}
void CDialogHistoryCurve::OnBnClickedOk()
{
	if (!clear())
	{
		return;
	}
}

void CDialogHistoryCurve::OnBnClickedCancel()
{
	if (!clear())
	{
		return;
	}

	OnCancel();
}

void CDialogHistoryCurve::OnShowVideo()
{
	if ( _indexVector.size() == 0 )
	{
		return;
	}

	osg::ref_ptr<RadarData> lpRD = NULL;
	{
		if( _curPos >= _indexVector.size() )
		{
			MessageBox(L"��ѯ����");
			return;
		}
		DataTemplateTab tab;
		tab.setProjectName( _projectName );
		std::stringstream ss;
		ss << tab.getSelectSql() << " where ID=" << _indexVector[_curPos];
		ss << " and ChannelIndex=" << _channelIndex;
		if ( !tab.open( RadarManager::Instance()->getDBHelp(), ss.str() ) )
		{
			return;
		}


		CDBTable::RowVector rv;

		tab.read( rv );

		if ( rv.size() == 0 )
		{
			return ;
		}

		lpRD = dynamic_cast<RadarData*>(rv[0].get());
		_radarWnd.clearData();
		_radarWnd.addData( lpRD );
		_radarWnd.Invalidate();

		osg::Vec3d pos = lpRD->getPosition();

		Kernel::FlyTo(pos.x(), pos.y(), pos.z() + 1000, 0, 45, 0 );
		float curLen = lpRD->getLen() + _startLen;

		CString str;
		str.Format(L"%0.3f", curLen);
		_curLenEdit.SetWindowText(str);
	}


	{

		
		VideoTemplateTab videoTab;
		videoTab.setProjectName( _projectName );

		std::stringstream ss;
		ss << videoTab.getSelectSql() << " where RECVTIME>=" << lpRD->getTime() - 3 << " AND RECVTIME<= "<< lpRD->getTime() + 3;

		if ( !videoTab.open( RadarManager::Instance()->getDBHelp(), ss.str() ) )
		{
			MessageBox(_T("����Ƶ�����"));
			return;
		}
		CDBTable::RowVector rv;

		videoTab.read( rv );

		if ( rv.size() == 0 )
		{
			MessageBox(_T("�Ҳ�����Ƶ��Ϣ"));
			return ;
		}

		//osg::ref_ptr<VideoTemplateTab::VideoRow> lpRow = dynamic_cast<VideoTemplateTab::VideoRow *>(rv[0].get());

		CWnd *lpWnd = RadarManager::Instance()->getVideoWnd();
		if ( !lpWnd )
		{
			return;
		}

		if ( !lpWnd->GetSafeHwnd() )
		{
			return;
		}

		USES_CONVERSION;
		for ( unsigned int i = 0; i < rv.size(); i ++ )
		{
			VideoTemplateTab::VideoRow *lpRow = dynamic_cast<VideoTemplateTab::VideoRow*>(rv[i].get());
			if ( !lpRow )
			{
				continue;
			}
			int cameraNum = lpRow->_cameraNum;
			CString filePath = A2W( lpRow->_filePath.c_str() );
			lpWnd->SendMessage( WM_USER + 1001, cameraNum, (LPARAM)&filePath );
		}


		// �ҵ�����Ƶ����, ����������ʾ��
	}

}

void CDialogHistoryCurve::OnShowPos()
{
	
	ShowData( true );


}
void CDialogHistoryCurve::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	_curPos = _radarWnd.GetHitPos( point );

	CMenu menu;
	menu.LoadMenu(IDR_MENU_HISTORY);

	menu.GetSubMenu(1)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, this);
}

void CDialogHistoryCurve::OnBnClickedButtonFind()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	UpdateData( TRUE );

	if ( _endPos < _startPos )
	{
		MessageBox(L"��ʼλ�ò���С�ڽ���λ��, �����¿�ʼ");
		return;
	}

	if ( _startPos - _startLen < 0 )
	{
		MessageBox(L"Ҫ��ѯ�Ŀ�ʼλ��С�ڹ��̿�ʼλ��, ����ͷ��ʼ��ѯ");
	}
	
	if ( _endPos - _startLen < 0 )
	{
		MessageBox(L"Ҫ��ѯ�Ŀ�ʼλ��С�ڹ��̿�ʼλ��, ����ѯ�����ݽ���");
	}

	if ( _startPos == _endPos )
	{
		MessageBox(L"λ������Ϊ0, ����ʾȫ��·������");
	}


	SetCursor( LoadCursor(NULL, IDC_WAIT) );
	init();
	SetCursor( LoadCursor(NULL, IDC_ARROW) );

	_radarWnd.Invalidate();
}

void CDialogHistoryCurve::OnBnClickedButtonExport()
{
	if ( !RadarManager::Instance()->dbOpen() )
	{
		return ;
	}
	DataTemplateTab tab;
	tab.setProjectName( _projectName );
	int count = 0;
	{
		DataCountTab countTab;
		countTab.setTabName( tab.getProjectName() );
		std::stringstream ss;
		ss << countTab.getSelectSql();
		if ( _startPos == _endPos )
		{
			ss << " where ChannelIndex=" << _channelIndex;
		}else
		{
			ss << " where ChannelIndex=" << _channelIndex;

			if ( _startPos - _startLen > 0 )
			{
				ss << " and LEN>=" << _startPos;
			}

			if ( _endPos - _startLen > 0 )
			{
				ss << " and LEN<=" << _endPos;
			}
		}

		count = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), ss.str());

	}

	if ( !_loadProcess )
	{
		_loadProcess = new CProcessDlg;
		_loadProcess->Create( CProcessDlg::IDD );
	}



	std::stringstream ss;
	if ( _startPos == _endPos )
	{
		ss << tab.getSelectSql() << " where ChannelIndex=" << _channelIndex;
	}else
	{
		ss << tab.getSelectSql() << " where ChannelIndex=" << _channelIndex;

		if ( _startPos - _startLen > 0 )
		{
			ss << " and LEN>=" << _startPos;
		}

		if ( _endPos - _startLen > 0 )
		{
			ss << " and LEN<=" << _endPos;
		}
	}

	ss << " order by id";

	std::string sqlStr = ss.str();
	if ( !tab.open( RadarManager::Instance()->getDBHelp(), sqlStr ) )
	{
		MessageBox(L"�򿪹��̱��ʧ��");
		return ;
	}

	CString sFilter = _T("SGY(*.SGY)|*.SGY|�����ļ�(*.SGY)||");

	CString str;
	
	USES_CONVERSION;
	str.Format(L"ͨ��%d-%s"
		, (_channelIndex + 1)
		, A2W(RadarManager::Instance()->getChannelName(_channelIndex).c_str() ));
	CFileDialog dlgSave(FALSE, L"SGY", str, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, (LPCTSTR)sFilter);
	if(dlgSave.DoModal() != IDOK)
	{
		return ;
	}


	CProcessDlg *lpDlg = (CProcessDlg*)_loadProcess;
	_loadProcess->ShowWindow( SW_SHOW );
	lpDlg->StartProcess( count );

	SetCursor( LoadCursor(NULL, IDC_WAIT) );

	CString saveName = dlgSave.GetPathName();
	CString fileName = dlgSave.GetFileName();
	CString saveExt = dlgSave.GetFileExt();
	if (saveExt != _T("SGY") && saveExt != _T("sgy"))
	{
		saveName += _T(".SGY");	//Ĭ��ʹ��scrconfig
	}
	std::string filePath = W2A( saveName );

	//����ļ�
	std::string pDisPicfilePath = filePath+".txt";
	const char* fileChar = pDisPicfilePath.c_str();
	std::ofstream infile(fileChar, std::ios::trunc);//������е�

	//SGY�ļ�
	SGYWriter writer;
	writer.openSGY( filePath, _xmlParam, _channelIndex );
	writer.writeHead( 1 ,_projectName);

	CDBTable::RowVector rv;

	int iBuf[1000];
	int lenc = 2048;
	unsigned char *data ;
	while(  ( rv = tab.read( 1000 )).size() != 0 )
	{
		for ( unsigned int i = 0; i < rv.size(); i ++ )
		{
			RadarData *lpRadarData = dynamic_cast<RadarData*>(rv[i].get());
			if ( lpRadarData )
			{
				writer.writeRadarData( lpRadarData );
				writer.writeDisLLPic(pDisPicfilePath, lpRadarData, infile);	
			}
			lpDlg->Step( 1 );

			data = lpRadarData->getData(lenc);
			iBuf[i] = data[6] + data[7] * 256 ;
			if (i > 1)
			{
				if (iBuf[i] < iBuf[i-1])
				{
					data = NULL;
				}
			}
			

		}


		//lpDlg->Step( rv.size() );
		if ( rv.size() < 1000 )
		{
			break;
		}
	}

	writer.closeSGY();
	SetCursor( LoadCursor(NULL, IDC_ARROW) );

	_loadProcess->ShowWindow( SW_HIDE );

	MessageBox(L"��������");
}


HRESULT CDialogHistoryCurve::OnClickCurve(WPARAM wParam, LPARAM lParam )
{
	if ( _indexVector.size() == 0 )
	{
		return 0;
	}
	CPoint p;
	p.x = ( int )wParam;
	p.y = (int )lParam;
	_curPos = _radarWnd.GetHitPos( p );
	ShowData( false );

	return 0;
}
void CDialogHistoryCurve::ShowData( bool location )
{
	if ( _indexVector.size() == 0 )
	{
		return;
	}
	if( _curPos >= _indexVector.size() )
	{
		MessageBox(L"��ѯ����");
		return;
	}


	DataTemplateTab tab;
	tab.setProjectName( _projectName );
	std::stringstream ss;
	ss << tab.getSelectSql() << " where ID=" << _indexVector[_curPos] << " and ChannelIndex=" << _channelIndex;
	if ( !tab.open( RadarManager::Instance()->getDBHelp(), ss.str() ) )
	{
		return;
	}

	CDBTable::RowVector rv;

	tab.read( rv );

	if ( rv.size() == 0 )
	{
		return ;
	}

	RadarData *lpRD = dynamic_cast<RadarData*>(rv[0].get());
	_radarWnd.clearData();
	_radarWnd.addData( lpRD );


	_radarWnd.Invalidate();

	osg::Vec3d pos = lpRD->getPosition();

	if ( location )
	{

		EMObj::CmdDelObj( GPS_OBJ_ID + 1);
		Kernel::DirectTo(pos.x(), pos.y(), pos.z() + 1000, 0, 45, 0 );


		osg::ref_ptr<Data::CRowShowObj> pRowShowObj = new Data::CRowShowObj;
		osg::Vec3d posXYZ;

		pRowShowObj->ID = GPS_OBJ_ID + 1;
		pRowShowObj->m_EnterID = 0;
		pRowShowObj->m_TypeID = 12;
		pRowShowObj->m_ParentID = 0;	
		pRowShowObj->m_Name = _T("GPSMODEL");
		pRowShowObj->m_LodConID = 0;//4;
		pRowShowObj->m_Visable = TRUE;

		CString objProperty;

		objProperty.Format(_T("<��Ļ��ע><ͼƬ����>green.png</ͼƬ����>\
							  <���뷽ʽ Type=\"CENTER\"></���뷽ʽ>\
							  <ͼƬ��С Height=\"24.00\" Type=\"�Զ���\" Width=\"24.00\"></ͼƬ��С>\
							  <ͼƬƫ�� offsetX=\"0.00\" offsetY=\"0.00\"></ͼƬƫ��>\
							  <�ı�����>���̲�����</�ı�����><��ʽģ�� NAME=\"\"></��ʽģ��>\
							  <���� FONT=\"SIMHEI.TTF\"></����><ԭ����뷽ʽ ALIGN=\"LEFT_CENTER\"></ԭ����뷽ʽ>\
							  <������ɫ A=\"100\" B=\"0\" G=\"0\" R=\"0\"></������ɫ>\
							  <�������� OFFSETX=\"0.00\" OFFSETY=\"0.00\" T=\"OUTLINE\"></��������>\
							  <�ֱ��� HEIGHT=\"128\" WIDTH=\"128\"></�ֱ���><���ִ�С S=\"24.00\"></���ִ�С>\
							  <������ɫ A=\"100\" B=\"128\" G=\"255\" R=\"255\"></������ɫ>\
							  <����ƫ�� offsetX=\"12.00\" offsetY=\"0.00\"></����ƫ��>\
							  <�ռ�λ�� X=\"%0.7f\" Y=\"%0.7f\" Z=\"%0.7f\"></�ռ�λ��>\
							  </��Ļ��ע>"),
							  pos.x(), pos.y(), pos.y()
							  );

		// 	objProperty.Format(_T("<ģ�� �ļ���='%s'><Pt3d x='%.14f' y='%.14f' z='%.14f'>λ��\
		// 						  </Pt3d><Pt3f x='%f' y='%f' z='%f'>�Ƕ���̬\
		// 						  </Pt3f><Pt3f x='%f' y='%f' z='%f'>����</Pt3f></ģ��>"),
		// 						  strPath,pos.x(),pos.y(),pos.z(),pose.x(),pose.y(),pose.z(),1.0,1.0,1.0);
		pRowShowObj->m_Property = objProperty;

		EMObj::CmdNewObjFromWeb(pRowShowObj);
	}


	float curLen = lpRD->getLen() + _startLen;

	CString str;
	str.Format(L"����:%0.3f;����:%0.7f,%0.7f,%0.7f", curLen,pos.x(), pos.y(), pos.y());
	_curLenEdit.SetWindowText(str);





}
void CDialogHistoryCurve::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if ( _lpLineNode.valid() )
	{
		COM::CCmdObjQueue *cmdQueue = NULL;
		if (Kernel::GetpKC() && (cmdQueue = Kernel::GetpKC()->m_CmdCenter.GetCmdObjList()))
		{
			osg::ref_ptr<HistoryLineCmd> pCmd = new HistoryLineCmd(HistoryLineCmd::T_DEL, _lpLineNode.get());
			cmdQueue->EnterQueue(pCmd);
		}

		EMObj::CmdDelObj( GPS_OBJ_ID + 1);
	}
	CDialog::OnClose();
}

void CDialogHistoryCurve::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	//static int a = 0;
	if (0 == m_iCount)
	{
		m_iCount++;
		return ;
	}

	CWnd* lWnd = NULL;
	int halfX = cx / 2;
	int halfY = cy / 2;
	int jiangeX = 3;


	int dlHight = 50;



	lWnd = GetDlgItem(IDC_CUSTOM_HISTORY_CURVE);
	lWnd->SetWindowPos( NULL,5,5,cx - 10 ,cy - dlHight ,
		SWP_NOZORDER  );
	lWnd->UpdateWindow();

	//lWnd->ShowWindow(SW_HIDE);


	dlHight = cy - dlHight + 15 ;
	lWnd = GetDlgItem(IDC_STATIC_BEGIN);
	lWnd->SetWindowPos( NULL,5,dlHight + jiangeX ,0 ,0,
		SWP_NOZORDER  | SWP_NOSIZE );
	lWnd->UpdateWindow();
	lWnd->SetWindowText(L"��ʼλ��");


	lWnd = GetDlgItem(IDC_EDIT_START);
	lWnd->SetWindowPos( NULL,60,dlHight,0 ,0,
		SWP_NOZORDER  | SWP_NOSIZE );
	lWnd->UpdateWindow();
	lWnd->UpdateData();
	

	lWnd = GetDlgItem(IDC_STATIC_END);
	lWnd->SetWindowPos( NULL,160,dlHight + jiangeX ,0 ,0,
		SWP_NOZORDER  | SWP_NOSIZE );
	lWnd->UpdateWindow();
	lWnd->SetWindowText(L"����λ��");

	lWnd = GetDlgItem(IDC_EDIT_END);
	lWnd->SetWindowPos( NULL,220,dlHight,0 ,0,
		SWP_NOZORDER  | SWP_NOSIZE );
	lWnd->UpdateWindow();
	lWnd->UpdateData();

	lWnd = GetDlgItem(IDC_BUTTON_FIND);
	lWnd->SetWindowPos( NULL,halfX - 90,dlHight,0 ,0,
		SWP_NOZORDER  | SWP_NOSIZE );
	lWnd->UpdateWindow();
	lWnd->SetWindowText(L"��ѯ");

	lWnd = GetDlgItem(IDC_BUTTON_EXPORT);
	lWnd->SetWindowPos( NULL,halfX + 5,dlHight,0 ,0,
		SWP_NOZORDER  | SWP_NOSIZE );
	lWnd->UpdateWindow();
	lWnd->SetWindowText(L"����");

	lWnd = GetDlgItem(IDC_STATIC_CHACKE);
	lWnd->SetWindowPos( NULL,cx - 440,dlHight + jiangeX ,0 ,0,
		SWP_NOZORDER  | SWP_NOSIZE );
	lWnd->UpdateWindow();
	lWnd->SetWindowText(L"ѡ�е�λ��");

	lWnd = GetDlgItem(IDC_EDIT_SELECT_POS);
	lWnd->SetWindowPos( NULL,cx - 360,dlHight,0 ,0,
		SWP_NOZORDER  | SWP_NOSIZE );
	lWnd->UpdateWindow();
	lWnd->UpdateData();
	
	
	



	UpdateData();
	UpdateWindow();


}

int CDialogHistoryCurve::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������
	//PostMessage(WM_SIZE,0,0);

	//UpdateWindow();
	return 0;
}

void CDialogHistoryCurve::ExportAllCHANNELToSGY()
{

	OnBnClickedButtonExport();
}