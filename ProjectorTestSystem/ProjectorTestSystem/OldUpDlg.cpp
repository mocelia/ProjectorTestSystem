// OldUpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ProjectorTestSystem.h"
#include "OldUpDlg.h"
#include "afxdialogex.h"


// COldUpDlg 对话框

/*全局变量*/
int OldFirstRow = 0;

IMPLEMENT_DYNAMIC(COldUpDlg, CDialogEx)

COldUpDlg::COldUpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COldUpDlg::IDD, pParent)
	, m_OldUpStatic(_T(""))
	, m_OldUpEditVal(_T(""))
{

}

COldUpDlg::~COldUpDlg()
{
}

void COldUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_OldUpList);
	DDX_Control(pDX, IDC_OLDUP_STATIC, m_OldUp);
	DDX_Text(pDX, IDC_OLDUP_STATIC, m_OldUpStatic);
	//DDX_Control(pDX, IDC_EDIT3, m_OldEdit);
	DDX_Control(pDX, IDC_OLDUPEDIT, m_OldUpEdit);
	DDX_Text(pDX, IDC_OLDUPEDIT, m_OldUpEditVal);
}


BEGIN_MESSAGE_MAP(COldUpDlg, CDialogEx)
END_MESSAGE_MAP()


// COldUpDlg 消息处理程序


BOOL COldUpDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_OldUpList.SetExtendedStyle(m_OldUpList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_OldUpList.InsertColumn(0, _T("机身码"), LVCFMT_CENTER, 150, 0);
	m_OldUpList.InsertColumn(1, _T("更新时间"), LVCFMT_CENTER, 150, 1);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

/*响应回车按键*/
BOOL COldUpDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	CString m_OldUpEditStr,OldUpSelectSql,OldUpTimeStr,OldUpUpdataSql;
	_variant_t FirstOldTime;
	int m_OldUpStaticLength;
	LONG OldUpRecordestCount;
	UpdateData(TRUE);
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (GetFocus()->GetDlgCtrlID() == IDC_OLDUPEDIT)
		{
			m_OldUpStaticLength = m_OldUpStatic.GetLength();
			m_OldUpEditStr = m_OldUpEditVal.Left(m_OldUpStaticLength);
			if (m_OldUpEditStr != m_OldUpStatic || m_OldUpEditVal=="")
			{
				MessageBox(_T("机身码错误"));
				m_OldUpEdit.SetFocus();
				m_OldUpEditVal = "";
				UpdateData(FALSE);
				return CDialogEx::PreTranslateMessage(pMsg);
			}
			OldUpSelectSql.Format(_T("select * from ProjectorInformation_MainTable where FuselageCode = '%s'"), m_OldUpEditVal);
			OperateDB.OpenRecordset(OldUpSelectSql);
			OldUpRecordestCount = OperateDB.GetRecordCount();
			if (OldUpRecordestCount == 0)
			{
				MessageBox(_T("不存在的机身码"));
				OperateDB.CloseRecordset();
				m_OldUpEdit.SetFocus();
				m_OldUpEditVal = "";
				UpdateData(FALSE);
				OperateDB.CloseRecordset();
				return CDialogEx::PreTranslateMessage(pMsg);
			}
			if (!OperateDB.m_pRecordset->BOF)
				OperateDB.m_pRecordset->MoveFirst();
			FirstOldTime = OperateDB.m_pRecordset->GetCollect(_T("PreAgingTestTime"));
			if (FirstOldTime.vt==VT_NULL)
			{
				MessageBox(_T("该产品未做老化前测试"));
				OperateDB.CloseRecordset();
				m_OldUpEdit.SetFocus();
				m_OldUpEditVal = "";
				UpdateData(FALSE);
				return CDialogEx::PreTranslateMessage(pMsg);
			}
			else
			{
				OldUpTimeStr = GetTime();
				OldUpUpdataSql.Format(_T("UPDATE ProjectorInformation_MainTable SET AgeingBeginTime = '%s' WHERE FuselageCode = '%s'"), OldUpTimeStr, m_OldUpEditVal);
				OperateDB.ExecuteByConnection(OldUpUpdataSql);
				m_OldUpList.InsertItem(OldFirstRow, m_OldUpEditVal);
				m_OldUpList.SetItemText(OldFirstRow, 1, OldUpTimeStr);
				OperateDB.CloseRecordset();
				m_OldUpEdit.SetFocus();
				m_OldUpEditVal = "";
				UpdateData(FALSE);
			}

		}
		OldFirstRow++;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

/*获取当前时间*/
CString COldUpDlg::GetTime()
{
	CTime time = CTime::GetCurrentTime();
	CString Tiemstr;
	Tiemstr = time.Format(_T("%Y-%m-%d  %H:%M:%S"));
	return Tiemstr;
}

void COldUpDlg::OnOK()
{
	// TODO:  在此添加专用代码和/或调用基类

	/*CDialogEx::OnOK();*/
}


void COldUpDlg::OnCancel()
{
	// TODO:  在此添加专用代码和/或调用基类

	/*CDialogEx::OnCancel();*/
}
