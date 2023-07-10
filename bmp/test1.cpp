#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Mat{
	int h,w;
public:
	Mat(int height,int width):h(height),w(width)
{ a++; num = a;}
		vector<vector<int>> data;
	friend istream& operator>>(istream& in, Mat& m){
		for(int i = 0; i < m.h; ++i){
			m.data.push_back(vector<int>{});
			for(int j = 0; j < m.w; ++j){
				int input;
				in >> input;
				m.data[i].push_back(input);
			}
		}
		return in;		
	}
	static int a;
	int num;
	friend ostream& operator<<(ostream& in, Mat& m){
		for(int i = 0; i < m.h; ++i){
			for(int j = 0; j < m.w; ++j){
				in << m.data[i][j] << " ";
			}
			in << endl;
		}	
		return in;
	}
	friend bool operator<(const Mat& a, const Mat& b){
		if((a.h * a.w) < (b.h * b.w)) return 1;
		else if((a.h * a.w) == (b.h * b.w)){
			if(a.num > b.num) return 1;
			else return 0;
		}
		else return 0;
	}
	friend bool comparator_1(const Mat& a, const Mat& b){
		int asum = 0, bsum = 0;
		for(int i = 0; i < a.h; ++i){
			for(int j = 0; j < a.w; ++j){
				asum += a.data[i][j];
			}
		}
		for(int i = 0; i < b.h; ++i){
			for(int j = 0; j < b.w; ++j){
				bsum += b.data[i][j];
			}
		}
		if(asum < bsum) return 1;
		else if(asum == bsum){
			if(a.num > b.num) return 1;
			else return 0;
		}
		else return 0;
	}
	friend class comparator_2;

};
class comparator_2{
public:
	bool operator()(const Mat& a, class Mat& b){
		return a.num < b.num;
	}
};
int Mat::a = 0;
int main()
{
	vector<Mat> m;
	m.push_back(Mat(2,2));
	m.push_back(Mat(3,4));
	m.push_back(Mat(2,2));
	cin >> m[0] >> m[1] >> m[2];
	sort(m.begin(),m.end());
	cout<< m[0] << endl << m[1] << endl << m[2] <<endl;
	cout<<"*************"<<endl;
	sort(m.begin(),m.end(),comparator_1);
	cout<< m[0] << endl << m[1] << endl << m[2] <<endl;
	cout<<"*************"<<endl;
	sort(m.begin(),m.end(),comparator_2());
	cout<< m[0] << endl << m[1] << endl << m[2] <<endl;
	return 0;
}