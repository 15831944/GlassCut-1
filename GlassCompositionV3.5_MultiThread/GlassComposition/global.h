#pragma once
class global
{
public:
	global(void);
	~global(void);
};

typedef struct DATA
{
	int subitem;// �����ͷ������
	CListCtrl* plist; //listctrl��ָ��
};

extern DATA* dat;
extern bool fav;