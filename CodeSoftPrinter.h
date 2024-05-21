#ifdef CODE_SOFT_PRINTER_EXPORTS
#define CODE_SOFT_PRINTER_API __declspec(dllexport)
#else
#define CODE_SOFT_PRINTER_API __declspec(dllimport)
#endif
#include <vector>
#include <string>
using namespace std;

/*****************************para in the vector********************************/
/*the first para in 'vecPara' is the path of *.Lab file                        */
/*the second para in 'vecPara' is the Name of printer(ex:ZDesigner GT800 (EPL))*/
/*the third para in 'vecPara' is the Port of printer (ex:USB003)               */

/*the rest para in 'vecPara' is the para of the .Lab File                      */
/******************************para in the vector*******************************/
CODE_SOFT_PRINTER_API void print_according_to_para(std::vector<std::string> vecPara);


