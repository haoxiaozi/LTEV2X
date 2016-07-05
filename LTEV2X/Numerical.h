#pragma once
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "Exception.h"

class Complex;//Complex��ǰ����������������4�������Ķ�����Complex�Ķ����໥������
double g_abs2(const Complex& a);
double g_abs(const Complex& a);
Complex g_Conjugate(const Complex& a);
double g_Factorial(int K);

class Complex {
public:
	/*���ݳ�Ա*/
	double m_Real;
	double m_Imag;

	/*���캯��*/
	Complex();
	Complex(double r);
	Complex(double r, double i);
	Complex(const Complex& a);//�������캯��
	~Complex();//��������

	/*��Ա���������*/
	Complex& operator =(const double& a);
	Complex& operator =(const Complex& c);
	Complex& operator +=(const Complex& c);
	Complex& operator -=(const Complex& c);
};
/*�����������������ΪComlex�ĳ�Ա����*/
Complex operator +(const Complex& c1, const Complex& c2);
Complex operator -(const Complex& c1, const Complex& c2);
Complex operator *(const Complex& c1, const Complex& c2);
Complex operator *(const Complex& c1, const double& a);
Complex operator /(const Complex& c1, const Complex& c2);
Complex operator /(const Complex& c1, const double& a);





class Matrix {
	static Complex s_A[64];
	static int s_pi[64];
	static Complex s_e[8];
	static Complex s_y[8];
	static int s_parity;
public:
	/*���ݳ�Ա*/
	int m_Rows;//����
	int m_Cols;//����
	Complex *m_Mem;//��ָ̬��
	
	/*���캯��*/
private:
	void copy(const Matrix& a);
public:
	void Matrix::initialize(int r, int c);//�����캯�����õĳ�ʼ������
	Matrix();
	Matrix(int r, int c);
	Matrix(const Matrix& m);//�������캯�������ڴ��ڶ�̬�����ָ�룬��Ҫ�Զ��忽������
	
	/*��Դ������*/
	void clean();//�ͷ���Դ
	~Matrix();//��������

	/*�����ܺ���*/
	void clear();//�����������Ԫ��
	Complex get(int row, int col);//���ؾ���ָ��λ�õ�Ԫ��
	void set(Complex a, int row, int col);//����ָ��λ��Ԫ�ظ�ֵ
	Matrix diag();//��ȡ����Խ���Ԫ��
	void eye();//�Ѿ����Ϊ��λ��
	Matrix trans();//�õ�����ת��
	Matrix hermitian();//�õ�������ת��
	Complex det();//��������ʽ
	Complex tr();//�������ļ�
	Matrix inv();//�õ�����������???
	Matrix inverse();//��������
	double norm_2();//����������
	void svd(Matrix* u, Matrix* s, Matrix* v);//SVD�ֽ�
	void appendInRow(const Matrix& a);//����������
	void appendInCol(const Matrix& a);//����������
	Matrix apart(int rs, int re, int cs, int ce);//???
	int _LUP_Decomposition(Complex* A, int n, int* pi);//???
	void _LUP_Solve(Complex* LU, int* pi, Complex *b, Complex* out, int n);//???
	

	/*��Ա�������������*/
	Matrix& operator =(const Matrix& m);
	Matrix& operator +=(const Matrix& m);
	Matrix& operator *=(const double& a);
	Matrix& operator /=(double a);
};
/*�����������������ΪComlex�ĳ�Ա����*/
Matrix operator + (const Matrix& m1,const Matrix& m2);
Matrix operator - (const Matrix& m1, const Matrix& m2);
Matrix operator *(const Matrix& m1, double a);
Matrix operator *(const Matrix& m1, const Matrix& m2);
Matrix operator /(const Matrix& m1, const double& a);
Matrix operator /(const Matrix& m1, const Complex& a);





