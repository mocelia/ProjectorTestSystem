// SetIndexDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ProjectorTestSystem.h"
#include "SetIndexDlg.h"
#include "afxdialogex.h"
#include "InDanNum.h"
#include "AdoDB.h"
#include "BefroeOldDlg.h"
#include "OldUpDlg.h"
#include "OldDownDlg.h"
#include "AfterOldDlg.h"
#include "BeforeBrightDlg.h"
#include "FixDlg.h"
#include "MainDlg.h"
#include "ProjectorTestSystemDlg.h"



/*ȫ�ֱ���*/
CString m_TypeName;
HTREEITEM IsSelect;
extern CProjectorTestSystemDlg * ProjectorTestSystemDlg;



//extern CSetIndexDlg * SetIndexDlg;
//extern CPloDlg * PloDlg;
// CSetIndexDlg �Ի���

IMPLEMENT_DYNAMIC(CSetIndexDlg, CDialogEx)

CSetIndexDlg::CSetIndexDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetIndexDlg::IDD, pParent)
{

}

CSetIndexDlg::~CSetIndexDlg()
{
}

void CSetIndexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_INDEXBUFF, m_SetIndex);
	DDX_Control(pDX, IDC_INDEXTYPE, m_IndexType);
	DDX_Control(pDX, IDC_TOOLINDEX, m_BodyNum);
	DDX_Control(pDX, IDC_SINGLETOOLINDEX, m_SingleBodyNum);
	DDX_Control(pDX, IDC_MAININDEX, m_MainBoardNum);
	DDX_Control(pDX, IDC_SAVE, m_Save);
	DDX_Control(pDX, IDC_ABORTION, m_Abortion);
	DDX_Control(pDX, IDC_CHANGE, m_Change);
	DDX_Control(pDX, IDC_DELETE, m_Delete);
	DDX_Control(pDX, IDC_SELECTINDEX, m_Select);
	DDX_Control(pDX, IDC_NEW, m_New);
}


BEGIN_MESSAGE_MAP(CSetIndexDlg, CDialogEx)
	ON_BN_CLICKED(IDC_SELECTINDEX, &CSetIndexDlg::OnBnClickedSelectindex)
	ON_BN_CLICKED(IDC_NEW, &CSetIndexDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_SAVE, &CSetIndexDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_ABORTION, &CSetIndexDlg::OnBnClickedAbortion)
	ON_BN_CLICKED(IDC_CHANGE, &CSetIndexDlg::OnBnClickedChange)
	ON_NOTIFY(TVN_SELCHANGED, IDC_INDEXBUFF, &CSetIndexDlg::OnTvnSelchangedIndexbuff)
	ON_BN_CLICKED(IDC_DELETE, &CSetIndexDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDOK, &CSetIndexDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetIndexDlg ��Ϣ��������

/*���붩����*/
void CSetIndexDlg::OnBnClickedSelectindex()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	CInDanNum InDanNum;
	INT_PTR InDanNumnRes;
	CString SubPrefixType;
	GetDlgItemText(IDC_INDEXTYPE, SubPrefixType);
	if (SubPrefixType == _T(""))
	{
		MessageBox(_T("ǰ׺����Ϊ�գ�������ѡ��"));
		return;
	}
	else
	{
		
		InDanNumnRes = InDanNum.DoModal();
		//InDanNum.m_InDanNum.SetFocus();
	}	
	if (InDanNumnRes==IDCANCEL)
	{
		return;
	}
}

/*�����ݿ�TypeName�ֶ�ֵ����tree contrl*/
BOOL CSetIndexDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ����Ӷ���ĳ�ʼ��	
	CString IndexName;
	OperateDB.OpenRecordset(_T("SELECT * FROM ProjectorInformation_EncodingRules"));
	hRoot = m_SetIndex.InsertItem(_T("ǰ׺"), NULL, NULL);
	try
	{
		if (!OperateDB.m_pRecordset->BOF)
			OperateDB.m_pRecordset->MoveFirst();
		else
		{
			AfxMessageBox(_T("��������Ϊ��"));
			OperateDB.CloseRecordset();
			return FALSE;
		}
		// ������и��ֶβ������б�����
		while (!OperateDB.m_pRecordset->adoEOF)
		{
			IndexName = OperateDB.m_pRecordset->GetCollect(_T("TypeName"));
			if (IndexName != _T(""))
			{
				m_SetIndex.InsertItem(IndexName, 1, 1, hRoot, TVI_LAST);
			}
			OperateDB.m_pRecordset->MoveNext();
		}
		OperateDB.CloseRecordset();
	}
	catch (_com_error &e)
	{
		OperateDB.m_szErrorMsg = e.ErrorMessage();
		return FALSE;
	}
	//SetIndexDlg = this;

	return FALSE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

/*�½���ť*/
void CSetIndexDlg::OnBnClickedNew()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	m_BodyNum.SetReadOnly(FALSE);
	m_SingleBodyNum.SetReadOnly(FALSE);
	m_MainBoardNum.SetReadOnly(FALSE);
	m_IndexType.SetReadOnly(FALSE);
	m_Save.EnableWindow(TRUE);
	m_Abortion.EnableWindow(TRUE);
	m_Change.EnableWindow(FALSE);
	m_Select.EnableWindow(FALSE);
	m_Delete.EnableWindow(FALSE);
	m_New.EnableWindow(FALSE);
	SetDlgItemText(IDC_TOOLINDEX,_T(""));
	SetDlgItemText(IDC_SINGLETOOLINDEX, _T(""));
	SetDlgItemText(IDC_INDEXTYPE, _T(""));
	SetDlgItemText(IDC_MAININDEX,_T(""));
	m_BodyNum.SetFocus();
}

/*���水ť*/
void CSetIndexDlg::OnBnClickedSave()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	CString Indextype, BodyNum, SingleBodyNum, MainBoardNum, IndexName,SqlUpdate;
	BOOL InsertFinshFlag = FALSE;
	
	GetDlgItemText(IDC_INDEXTYPE,Indextype);
	GetDlgItemText(IDC_TOOLINDEX, BodyNum);
	GetDlgItemText(IDC_SINGLETOOLINDEX, SingleBodyNum);
	GetDlgItemText(IDC_MAININDEX, MainBoardNum);
	if (Indextype==m_TypeName)
	{
		SqlUpdate.Format(_T("UPDATE ProjectorInformation_EncodingRules SET Prefix_BodyCode ='%s',Prefix_OpticalCode='%s',Prefix_MotherboardEncoding='%s' WHERE TypeName = '%s'"), BodyNum, SingleBodyNum, MainBoardNum, Indextype);
		InsertFinshFlag = OperateDB.ExecuteByConnection(SqlUpdate);
	}
	else
	{
		InsertFinshFlag = OperateDB.ExecuteByConnection(_T("insert into ProjectorInformation_EncodingRules\
										(TypeName,Prefix_BodyCode,Prefix_OpticalCode,Prefix_MotherboardEncoding)\
										values('" + Indextype + "'," + BodyNum + "," + SingleBodyNum + "," + MainBoardNum + ")"));
	}
	
	if (InsertFinshFlag==TRUE)
	{
		m_SetIndex.DeleteAllItems();
		hRoot = m_SetIndex.InsertItem(_T("ǰ׺"), NULL, NULL);
		OperateDB.OpenRecordset(_T("SELECT * FROM ProjectorInformation_EncodingRules"));
		try
		{
			if (!OperateDB.m_pRecordset->BOF)
				OperateDB.m_pRecordset->MoveFirst();
			else
			{
				AfxMessageBox(_T("��������Ϊ��"));
				return;
			}
			// ������и��ֶβ������б�����
			while (!OperateDB.m_pRecordset->adoEOF)
			{
				IndexName = OperateDB.m_pRecordset->GetCollect(_T("TypeName"));
				if (IndexName != _T(""))
				{
					m_SetIndex.InsertItem(IndexName, 1, 1, hRoot, TVI_LAST);
				}
				OperateDB.m_pRecordset->MoveNext();
			}
			OperateDB.CloseRecordset();
		}
		catch (_com_error &e)
		{
			OperateDB.m_szErrorMsg = e.ErrorMessage();
			return;
		}
		MessageBox(_T("����ɹ���"));
		OperateDB.CloseRecordset();
		m_BodyNum.SetReadOnly(TRUE);
		m_SingleBodyNum.SetReadOnly(TRUE);
		m_MainBoardNum.SetReadOnly(TRUE);
		m_IndexType.SetReadOnly(TRUE);
		m_Save.EnableWindow(FALSE);
		m_Abortion.EnableWindow(FALSE);
		m_Change.EnableWindow(TRUE);
		m_Select.EnableWindow(TRUE);
		m_Delete.EnableWindow(TRUE);
		m_New.EnableWindow(TRUE);
		m_SetIndex.EnableWindow(TRUE);
	}
	else
	{
		MessageBox(_T("����ʧ�ܣ��������޸ģ�"));
	}	
}

/*ȡ����ť*/
void CSetIndexDlg::OnBnClickedAbortion()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	SetDlgItemText(IDC_TOOLINDEX, _T(""));
	SetDlgItemText(IDC_SINGLETOOLINDEX, _T(""));
	SetDlgItemText(IDC_INDEXTYPE, _T(""));
	SetDlgItemText(IDC_MAININDEX, _T(""));
	m_BodyNum.SetReadOnly(TRUE);
	m_SingleBodyNum.SetReadOnly(TRUE);
	m_MainBoardNum.SetReadOnly(TRUE);
	m_IndexType.SetReadOnly(TRUE);
	m_Save.EnableWindow(FALSE);
	m_Abortion.EnableWindow(FALSE);
	m_Change.EnableWindow(TRUE);
	m_Select.EnableWindow(TRUE);
	m_Delete.EnableWindow(TRUE);
	m_New.EnableWindow(TRUE);	
}

/*�޸İ�ť*/
void CSetIndexDlg::OnBnClickedChange()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	m_Save.EnableWindow(TRUE);
	m_Abortion.EnableWindow(TRUE);
	m_Change.EnableWindow(FALSE);
	m_Select.EnableWindow(FALSE);
	m_Delete.EnableWindow(FALSE);
	m_New.EnableWindow(FALSE);
	m_BodyNum.SetReadOnly(FALSE);
	m_SingleBodyNum.SetReadOnly(FALSE);
	m_MainBoardNum.SetReadOnly(FALSE);
	m_SetIndex.EnableWindow(FALSE);
}

/*���οؼ���Ӧ����*/
void CSetIndexDlg::OnTvnSelchangedIndexbuff(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	CString BodyNum, SingleBodyNum, MainBoardNum,SqlStr;
	IsSelect = m_SetIndex.GetSelectedItem();
	m_TypeName = m_SetIndex.GetItemText(IsSelect);
	if (IsSelect==hRoot)
	{
		return;
	}
	SqlStr.Format(_T("select Prefix_BodyCode,Prefix_OpticalCode,Prefix_MotherboardEncoding from ProjectorInformation_EncodingRules where TypeName='%s'"), m_TypeName);
	OperateDB.OpenRecordset(SqlStr);
	BodyNum = OperateDB.m_pRecordset->GetCollect(_T("Prefix_BodyCode"));
	SingleBodyNum = OperateDB.m_pRecordset->GetCollect(_T("Prefix_OpticalCode"));
	MainBoardNum = OperateDB.m_pRecordset->GetCollect(_T("Prefix_MotherboardEncoding"));
	SetDlgItemText(IDC_INDEXTYPE, m_TypeName);
	SetDlgItemText(IDC_TOOLINDEX, BodyNum);
	SetDlgItemText(IDC_SINGLETOOLINDEX, SingleBodyNum);
	SetDlgItemText(IDC_MAININDEX, MainBoardNum);
	OperateDB.CloseRecordset();
	*pResult = 0;
}

/*ɾ����ť*/
void CSetIndexDlg::OnBnClickedDelete()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	CString SqlDelete;
	BOOL DeleteFinshFlag = FALSE;
	SqlDelete.Format(_T("DELETE FROM ProjectorInformation_EncodingRules WHERE TypeName = '%s'"), m_TypeName);
	DeleteFinshFlag = OperateDB.ExecuteByConnection(SqlDelete);	
	if (DeleteFinshFlag==TRUE)
	{
		m_SetIndex.DeleteItem(IsSelect);
		MessageBox(_T("ɾ���ɹ�"));
	}
	else
	{
		MessageBox(_T("ɾ��ʧ��"));
	}
}

/*ȷ�ϼ�*/
void CSetIndexDlg::OnBnClickedOk()
{
	// TODO:  �ڴ����ӿؼ�֪ͨ�����������
	GetDlgItemText(IDC_INDEXTYPE, PrefixType);
	GetDlgItemText(IDC_TOOLINDEX, BodyNumPrefix);
	GetDlgItemText(IDC_SINGLETOOLINDEX, SingleBodyNumPrefix);
	GetDlgItemText(IDC_MAININDEX, MainBoardNumPrefix);
	ProjectorTestSystemDlg->m_Plo.SetDlgItemText(IDC_PLO_BODYNUM_STATIC, BodyNumPrefix);
	ProjectorTestSystemDlg->m_Plo.SetDlgItemText(IDC_PLO_SINGLEBODYNUM_STATIC, SingleBodyNumPrefix);
	ProjectorTestSystemDlg->m_Plo.SetDlgItemText(IDC_MAINBOARDNUM_STATIC, MainBoardNumPrefix);
	ProjectorTestSystemDlg->m_BeforeOld.SetDlgItemText(IDC_BEFOREOLD_STATIC, BodyNumPrefix);
	ProjectorTestSystemDlg->m_OldUp.SetDlgItemText(IDC_OLDUP_STATIC, BodyNumPrefix);
	ProjectorTestSystemDlg->m_OldDown.SetDlgItemText(IDC_OLDDOWN_STATIC, BodyNumPrefix);
	ProjectorTestSystemDlg->m_AfterOld.SetDlgItemText(IDC_AFTEROLD_STATIC, BodyNumPrefix);
	ProjectorTestSystemDlg->m_BeforeBright.SetDlgItemText(IDC_BEFOREBRIGHT_STATIC, BodyNumPrefix);
	ProjectorTestSystemDlg->m_Fix.SetDlgItemText(IDC_FIX_STATIC, BodyNumPrefix);
	ProjectorTestSystemDlg->m_Fix.SetDlgItemText(IDC_FIX_SINGLEBODYNUM_STATIC,SingleBodyNumPrefix);
	ProjectorTestSystemDlg->m_Fix.SetDlgItemTextA(IDC_FIX_MAINBOARDNUM_STATIC,MainBoardNumPrefix);
	ProjectorTestSystemDlg->m_Pack.SetDlgItemText(IDC_PACK_STATIC, BodyNumPrefix);
	ProjectorTestSystemDlg->m_Plo.SetDlgItemText(IDC_ZHIDANNUM,DanNum);
	CDialogEx::OnOK();
}