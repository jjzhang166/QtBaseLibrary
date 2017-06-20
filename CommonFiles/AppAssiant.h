#pragma once

//辅助的检测宏：简化代码的编写
#define CHK_EXP_ACT( exp , one , two )    if( exp ){ one ; } else { two ; }
#define CHK_EXP_RET( exp , ret )        CHK_EXP_ACT( exp , return ret , ; )
#define CHK_EXP_RUN( exp , run )        CHK_EXP_ACT( exp , run , ; )
#define CHK_EXP_END( exp )              CHK_EXP_RUN( exp , return )


//释放内存使用
#define CHK_POINT_DESTORY( point )      if( point ) { delete point; point = NULL; }
