#pragma once
class Tank
{
public:
	
	CString Name;						//��ũ �̸�
	int Size;							//��ũ ũ��
	int Kind;							//��ũ ����
	int Num;							//��ũ ��ȣ(�ε���)
	int Bomb;							//���õ� ��ũ�� ��ź
	int Bullet;							//���õ� ��ũ�� ��
	int Speed;							//��ũ�� �ӵ�
	int Health;							//��ũ�� ü��
	int Defense;						//��ũ�� ����
	int Damage;							//��ũ�� ���ݷ�
	int JumpDegree;						//��ũ�� ������
	int X,H;							//��ũ�� ���� ��ġ�� X��° ���� ����
	int BulletSleepTime;				//��ź �߻� �� ���ʵ��� �� ���ΰ�?
	int BombSleepTime;					//��ź...?
	int JumpLevel;						//������ �� y��ǥ
	int JumpSleepTime;					//���� �� ���ʵ��� �� ���ΰ�?
	int JumpTime;						//���� �� �lŸ��(����)
	int BulletTime;						//��ź �߻� �� ��Ÿ��
	int BombTime;						//��ź �߻� �� ��Ÿ��
	int MaxAngel, MinAngel;				//��ũ������ �ּҰ��� �ִ밢��			
	LONG PowerCoords;					//*���� ������ �������� ���������� ��ǥ
	BOOL Sign,Fire;						//������ �ȳ�����, �������� �������, ����� �� �������� Fire�� ����
	BOOL dx;							//��ũ�� ���ϰ� �ִ� ����
	BOOL Landing;						//�ٴ��� ���� �� ����
	BOOL Jump, Fall, Angel, Power;		//��ũ�� ����, ����, �߻翩��
	BOOL JumpSleep,BulletSleep,BombSleep;		//����, ��ź, ��ź�� ���ɿ���				
	BOOL EntryLeft, EntryRight,NoEntry;	//���Կ���
	CPoint Coords;						//*��ũ�� ���� ��ǥ
	CPoint AngelCoords;					//*������ ������ �������� �����ȳ����� ��ǥ
	CBitmap TankBmp;					//��ũ�� ��Ʈ��
	BITMAP TankInfo;
	
	
public:
	Tank();
	~Tank(void);
	void SetBitmap(int Tank);
	void SetTank(int x);


};

