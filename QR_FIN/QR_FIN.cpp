#include "zbar.h"           
#include <opencv2/core.hpp> 
#include "opencv2/imgproc/imgproc_c.h"
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include "iostream"
#include "fstream"
#include "opencv2/core/core.hpp" 
#include "opencv2/highgui/highgui.hpp" 
#include "opencv2/imgproc/imgproc.hpp" 

using namespace std;
using namespace zbar;
using namespace cv;

int main(int argc, char* argv[])
{

	VideoCapture VC(0);
	while (true)
	{
		ImageScanner scanner;
		scanner.set_config(ZBAR_NONE, ZBAR_CFG_ENABLE, 1);

		Mat image;
		VC.read(image);
		imshow("Source Image", image);
		if (!image.data)
		{
			cout << "请确认摄像头正确采集" << endl;
			system("pause");
			return 0;
		}
		Mat imageGray;
		cvtColor(image, imageGray, CV_RGB2GRAY);
		int width = imageGray.cols;
		int height = imageGray.rows;
		uchar* raw = (uchar*)imageGray.data;
		Image imageZbar(width, height, "Y800", raw, width * height);
		scanner.scan(imageZbar); //扫描条码      
		Image::SymbolIterator symbol = imageZbar.symbol_begin();
		if (imageZbar.symbol_begin() == imageZbar.symbol_end())
		{
			cout << "查询条码失败，请检查图片！" << endl;
		}
		else
		{
			for (; symbol != imageZbar.symbol_end(); ++symbol)
			{
				cout << "类型：" << endl << symbol->get_type_name() << endl << endl;
				cout << "条码：" << endl << symbol->get_data() << endl << endl;
				//将检测的结果写到result.txt中方便查阅，追加方式写入的，
				ofstream fout("result.txt", ios::app);
				fout << "类型：" << symbol->get_type_name() << endl << "条码：" << symbol->get_data() << endl << endl;
				fout.close();
				int key = waitKey();
				if (key == 27) return 0;
			}
		}

		imageZbar.set_data(NULL, 0);
		int key = waitKey(100);
		if (key == 27) return 0;
	}
	waitKey();
	return 0;
}