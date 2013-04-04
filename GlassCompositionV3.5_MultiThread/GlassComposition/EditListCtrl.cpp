// EditListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "GlassComposition.h"
#include "GlassCompositionDlg.h"
#include "EditListCtrl.h"

//extern double totalArea;
// CEditListCtrl

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)

	CEditListCtrl::CEditListCtrl()
{
	m_edit.m_hWnd = NULL;
}

CEditListCtrl::~CEditListCtrl()
{
}

BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	ON_WM_LBUTTONDBLCLK()
	ON_NOTIFY_REFLECT(NM_CLICK, &CEditListCtrl::OnNMClick)
	ON_MESSAGE(WM_USER_EDIT_END, OnEditEnd)
END_MESSAGE_MAP()


void CEditListCtrl::ShowEdit(BOOL bShow , int nItem, int nIndex, CRect rcCtrl)
{
	//AfxMessageBox("ShowEdit");
	// ����༭�������δ����
	if(m_edit.m_hWnd == NULL)
	{
		//����һ���༭�򣨴�СΪ�㣩
		m_edit.Create(ES_AUTOHSCROLL|WS_CHILD|ES_LEFT
			|ES_WANTRETURN|WS_BORDER,CRect(0,0,0,0),this,IDC_EDIT_HIDE);
		m_edit.ShowWindow(SW_HIDE);// ����

		//ʹ��Ĭ������
		CFont tpFont;
		tpFont.CreateStockObject(DEFAULT_GUI_FONT);
		m_edit.SetFont(&tpFont);
		tpFont.DeleteObject();
	}
	//���bShowΪtrue����ʾ�༭��
	if(bShow == TRUE)
	{
		CString strItem = CListCtrl::GetItemText(nItem,nIndex);//��ȡ�б�ؼ��������������
		m_edit.MoveWindow(rcCtrl);// �ƶ���������������
		m_edit.ShowWindow(SW_SHOW);//��ʾ�ؼ�		
		m_edit.SetWindowText(strItem);// ��ʾ����
		::SetFocus(m_edit.GetSafeHwnd());//���ý���
		DWORD dt = nItem << 16 | nIndex;
		m_edit.SetCtrlData(dt);
		::SendMessage(m_edit.GetSafeHwnd(), EM_SETSEL, 0, -1);//ʹ���ݴ���ѡ��״̬
	}
	else
		m_edit.ShowWindow(SW_HIDE);
}




void CEditListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//AfxMessageBox("OnLButtonDbClk");
	CRect rcCtrl;        //��������������
	LVHITTESTINFO lvhti; //�����б�ؼ�������������Ե����ݽṹ
	lvhti.pt = point;  //�������λ��
	int nItem = CListCtrl::SubItemHitTest(&lvhti);//���û����������Ժ����������к�
	if(nItem == -1)   //�������ڿؼ���˫���������κδ���
		return;
	int nSubItem = lvhti.iSubItem;//����к�
	if (nSubItem != 4)
	{
		CListCtrl::GetSubItemRect(nItem,nSubItem,LVIR_LABEL,rcCtrl);
		//��������������򣬴���rcCtrl
		ShowEdit(TRUE,nItem,nSubItem,rcCtrl); //�����Զ��庯������ʾ�༭��
	}

	CListCtrl::OnLButtonDblClk(nFlags, point);
}


void CEditListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here

	if(m_edit.m_hWnd != NULL)
	{
		DWORD dwStyle = m_edit.GetStyle();
		if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
		{
			m_edit.ShowWindow(SW_HIDE);
		}
	} 

	*pResult = 0;
}


LRESULT CEditListCtrl::OnEditEnd(WPARAM wParam , LPARAM lParam)
{

	if(wParam == TRUE)
	{
		CString strText(_T(""));
		m_edit.GetWindowText(strText);
		DWORD dwData = m_edit.GetCtrlData();
		int nItem= dwData>>16;
		CString st;
		st.Format("%d", nItem);
		//AfxMessageBox(st);
		int nIndex = dwData&0x0000ffff;
		st.Format("%d", nIndex);
		//AfxMessageBox(st);
		CListCtrl::SetItemText(nItem,nIndex,strText);

		CWnd *pwnd = AfxGetMainWnd();

		if (nIndex == 1)
		{
			((CGlassCompositionDlg *)pwnd)->v_Id[nItem] = atoi(CListCtrl::GetItemText(nItem, 1));
			if (!(((CGlassCompositionDlg *)pwnd)->JudgeSameID(((CGlassCompositionDlg *)pwnd)->v_Id)))
			{
				AfxMessageBox("����ظ�������");
			}
		}

		if (nIndex == 2 || nIndex == 3)
		{
			CString area_cur = CListCtrl::GetItemText(nItem, 6);
			double area_cur_t = atof(area_cur);
			//CGlassCompositionDlg *pDlg = (CGlassCompositionDlg *)GetParent()->GetParent();
			//CWnd *pwnd = AfxGetMainWnd();
			double totla = ((CGlassCompositionDlg *)pwnd)->totalArea;
			totla -= area_cur_t;
			//CString a = theApp.m_pMainWnd->GetWindowText(IDC_TOTAL_AREA);
			//totalArea -= area_cur_t;
			CString str_len, str_wid, str_area, str_area_tt;
			int len, wid, ae;
			double ae_tt;
			str_len = CListCtrl::GetItemText(nItem, 2);
			str_wid = CListCtrl::GetItemText(nItem, 3);
			len = atoi(str_len);
			wid = atoi(str_wid);
			ae = len * wid;
			ae_tt = (double)ae * atoi(CListCtrl::GetItemText(nItem, 5)) / 100;
			str_area.Format("%d", ae);
			CListCtrl::SetItemText(nItem, 4, str_area);
			str_area_tt.Format("%.2lf", ae_tt);
			CListCtrl::SetItemText(nItem, 6, str_area_tt);

			CString ta;
			totla += ae_tt;
			((CGlassCompositionDlg *)pwnd)->totalArea = totla;
			ta.Format("%.2lf", totla);
			((CGlassCompositionDlg *)pwnd)->GetDlgItem(IDC_TOTAL_AREA)->SetWindowText(ta);
		}
		if (nIndex == 5)
		{
			//--------------
			CString area_cur = CListCtrl::GetItemText(nItem, 6);
			double area_cur_t = atof(area_cur);
			CWnd *pwnd = AfxGetMainWnd();
			double totla = ((CGlassCompositionDlg *)pwnd)->totalArea;
			totla -= area_cur_t;

			//CString need_num = CListCtrl::GetItemText(nItem, 5);
			//int nn = atoi(need_num);
			int tnn = ((CGlassCompositionDlg *)pwnd)->totalNeedNumber;
			tnn -= ((CGlassCompositionDlg *)pwnd)->nn_edit[nItem];
			//--------------
			CString str_area, str_area_tt;
			int ae;
			double ae_tt;
			str_area = CListCtrl::GetItemText(nItem, 4);
			ae = atoi(str_area);
			ae_tt = (double)ae * atoi(CListCtrl::GetItemText(nItem, 5)) / (double)100;
			str_area_tt.Format("%.2lf", ae_tt);
			CListCtrl::SetItemText(nItem, 6, str_area_tt);

			CString ta;
			totla += ae_tt;
			((CGlassCompositionDlg *)pwnd)->totalArea = totla;
			ta.Format("%.2lf", totla);
			((CGlassCompositionDlg *)pwnd)->GetDlgItem(IDC_TOTAL_AREA)->SetWindowText(ta);

			CString tn;
			tnn += atoi(CListCtrl::GetItemText(nItem, 5));
			((CGlassCompositionDlg *)pwnd)->totalNeedNumber = tnn;
			tn.Format("%d", tnn);
			((CGlassCompositionDlg *)pwnd)->GetDlgItem(IDC_TOTAL_NUMBER)->SetWindowText(tn);
		}
		((CGlassCompositionDlg *)pwnd)->vv_Id[nItem][0] = atoi(CListCtrl::GetItemText(nItem, 2));
		((CGlassCompositionDlg *)pwnd)->vv_Id[nItem][1] = atoi(CListCtrl::GetItemText(nItem, 3));
		((CGlassCompositionDlg *)pwnd)->vv_Id[nItem][2] = atoi(CListCtrl::GetItemText(nItem, 5));
		if (!(((CGlassCompositionDlg *)pwnd)->JudgeSameValue(((CGlassCompositionDlg *)pwnd)->vv_Id)))
		{
			AfxMessageBox("�����ظ�����Ƭ��������");
		}
	}
	else
	{   
	}
	if(lParam == FALSE)
		m_edit.ShowWindow(SW_HIDE);
	return 0;

}


BOOL CEditListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_KEYDOWN)
	{		
		//����Tab��
		if(pMsg->wParam == VK_TAB && m_edit.m_hWnd!= NULL)
		{
			//���༭���Ƿ�����ʾ״̬
			DWORD dwStyle = m_edit.GetStyle();
			if((dwStyle&WS_VISIBLE) == WS_VISIBLE)
			{
				OnEditEnd(TRUE,TRUE);//����ǰһ�����������
				CRect rcCtrl;  
				int nItem;
				int nSub;
				//if(FALSE == Key_Ctrl(nItem,nSub)) //ʵ��Ctrl+Tab�Ĺ���
				Key_Shift(nItem,nSub);//����Key_Shift�����кż��к�
				//�����ת����������
				CListCtrl::GetSubItemRect(nItem,nSub,LVIR_LABEL,rcCtrl);
				//����༭״̬
				CPoint pt(rcCtrl.left+1,rcCtrl.top+1);
				OnLButtonDblClk(0,pt);
				//�����б�ѡ��״̬
				POSITION pos = CListCtrl::GetFirstSelectedItemPosition();
				if (pos == NULL)
				{ }
				else
				{
					while (pos)
					{
						int ntpItem = CListCtrl::GetNextSelectedItem(pos);
						CListCtrl::SetItemState(ntpItem,0,LVIS_SELECTED);
					}
				}
				CListCtrl::SetItemState(nItem,  LVIS_SELECTED,  LVIS_SELECTED);
				return TRUE;
			}
		}
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}


void CEditListCtrl::Key_Shift(int & nItem, int & nSub)
{
	//�б�������
	int nItemCount = CListCtrl::GetItemCount();
	//��ǰ�༭������λ��
	DWORD dwData = m_edit.GetCtrlData();
	nItem= dwData>>16;
	nSub = dwData&0x0000ffff;
	//��ȡ����ؼ�ָ��
	CHeaderCtrl* pHeader = CListCtrl::GetHeaderCtrl();
	if(pHeader == NULL)
		return;

	// ���SHIFT����״̬�����λΪ1-������0-δ����
	short sRet = GetKeyState(VK_SHIFT);
	int nSubcCount = pHeader->GetItemCount();//������
	sRet = sRet >>15;

	if(sRet == 0)//δ����
	{
		nSub += 1;//�кŵ���
		if(nSub >= nSubcCount)//����β
		{
			if(nItem == nItemCount-1)//����β�����ر�ͷ
			{
				nItem = 0;
				nSub  = 0;
			}else //δ����β��������һ������
			{
				nSub = 0;
				nItem += 1;
			}
		}

		if(nItem >= nItemCount)
			nItem = nItemCount-1;
	}
	else//����
	{
		nSub -= 1;//�кŵݼ�
		if(nSub < 0)//������,������һ����β
		{
			nSub = nSubcCount -1;
			nItem --;
		}
		if(nItem < 0)//�����ף�������β
			nItem = nItemCount-1;
	}

}


BOOL CEditListCtrl::Key_Ctrl(int & nItem, int & nSub)
{
	short sRet = GetKeyState(VK_CONTROL);
	DWORD dwData = m_edit.GetCtrlData();
	nItem= dwData>>16;
	nSub = dwData&0x0000ffff;

	sRet = sRet >>15;
	int nItemCount = CListCtrl::GetItemCount();
	if(sRet != 0)
	{
		nItem = nItem >=nItemCount-1? 0:nItem+=1;
		return TRUE;
	}
	return FALSE;

}
