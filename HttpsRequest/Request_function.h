#pragma once

#include <iostream>
#include <string>  
#include <stdio.h>
#include <VMProtectSDK.h>

#ifdef WIN32
#pragma comment(lib,"ws2_32.lib")
#endif

#include "Https_Request.hpp"
#include "MachineCodeGenerator.h"

using namespace std;


/// hyxdQuick授权码登录
inline std::string softids = VMProtectDecryptStringA("7F6R0K3U9H9V7B0R");//软件标识
inline std::string versions = VMProtectDecryptStringA("Bitch");//版本
inline std::string cards = VMProtectDecryptStringA("Fuck");//hyxdQuick授权码
inline std::string Macs = MachineCodeGenerator::GenerateMachineCode();//机器码

/// 用户登录
inline std::string UserName;//用户名
inline std::string UserPwd;//密码
inline std::string SupPwd;//超级密码
inline std::string Recommender;//推荐人

/// 返回信息
inline std::string str;
inline std::string LoginToken;



namespace login
{
	inline std::map<std::string, std::string> errorMap = {
	{VMProtectDecryptStringA("-81001"), VMProtectDecryptStringA("Api接口不存在.")},
	{VMProtectDecryptStringA("-81003"), VMProtectDecryptStringA("hyxdQuick已被强制关闭.")},
	{VMProtectDecryptStringA("-81004"), VMProtectDecryptStringA("hyxdQuick不存在.")},
	{VMProtectDecryptStringA("-81005"), VMProtectDecryptStringA("hyxdQuick已停用.")},
	{VMProtectDecryptStringA("-81006"), VMProtectDecryptStringA("hyxdQuick版本不存在.")},
	{VMProtectDecryptStringA("-81007"), VMProtectDecryptStringA("hyxdQuick接口密码错误.")},
	{VMProtectDecryptStringA("-81008"), VMProtectDecryptStringA("hyxdQuick不允许用户转绑.")},
	{VMProtectDecryptStringA("-81009"), VMProtectDecryptStringA("hyxdQuick已开启收费模式,当前版本为免费版本.")},
	{VMProtectDecryptStringA("-81010"), VMProtectDecryptStringA("hyxdQuick版本已停用.")},
	{VMProtectDecryptStringA("-81011"), VMProtectDecryptStringA("hyxdQuick版本必须为数字.")},
	{VMProtectDecryptStringA("-81012"), VMProtectDecryptStringA("hyxdQuick未开启试用功能.")},
	{VMProtectDecryptStringA("-81015"), VMProtectDecryptStringA("频繁调用,请等待10分钟后再做尝试.")},
	{VMProtectDecryptStringA("-81016"), VMProtectDecryptStringA("单IP频繁访问限制.")},
	{VMProtectDecryptStringA("-81017"), VMProtectDecryptStringA("此接口开启了算法,但是提交时未加密数据再提交.")},
	{VMProtectDecryptStringA("-81018"), VMProtectDecryptStringA("变量数据不存在.")},
	{VMProtectDecryptStringA("-81019"), VMProtectDecryptStringA("变量ID必须为数字.")},
	{VMProtectDecryptStringA("-81020"), VMProtectDecryptStringA("变量别名只能是数字或者字母或者数字字母混合.")},
	{VMProtectDecryptStringA("-81021"), VMProtectDecryptStringA("错误的调用方式,请查看后台接口的调用方式和参数.")},
	{VMProtectDecryptStringA("-81022"), VMProtectDecryptStringA("机器码填写错误，长度必须是不能超过32位的字母或数字.")},
	{VMProtectDecryptStringA("-81023"), VMProtectDecryptStringA("请先登陆后再调用此方法或账号已被顶下线.")},
	{VMProtectDecryptStringA("-81024"), VMProtectDecryptStringA("扣点数量必须大于0.")},
	{VMProtectDecryptStringA("-81025"), VMProtectDecryptStringA("软件扣费模式和卡类不符.")},
	{VMProtectDecryptStringA("-81026"), VMProtectDecryptStringA("当前扣费模式为时间计费,无法使用扣点功能.")},
	{VMProtectDecryptStringA("-81027"), VMProtectDecryptStringA("黑名单已存在.")},
	{VMProtectDecryptStringA("-81028"), VMProtectDecryptStringA("当前用户在别的机器登陆使用,暂时无法登陆 请等10分钟.")},
	{VMProtectDecryptStringA("-81029"), VMProtectDecryptStringA("当前用户在别的IP登陆使用,暂时无法登陆 请等10分钟.")},
	{VMProtectDecryptStringA("-81030"), VMProtectDecryptStringA("当前用户已超过最大登陆数量,暂时无法登陆 请等10分钟.")},
	{VMProtectDecryptStringA("-81031"), VMProtectDecryptStringA("用户已在别的地方登陆.")},
	{VMProtectDecryptStringA("-81032"), VMProtectDecryptStringA("token填写错误,长度必须是16位字母或数字.")},
	{VMProtectDecryptStringA("-81033"), VMProtectDecryptStringA("用户名或hyxdQuick授权码或试用特征填写错误,长度必须是6-16位的字母或数字.")},
	{VMProtectDecryptStringA("-81035"), VMProtectDecryptStringA("解绑类型只能为数字并且只能为1或者2")},
	{VMProtectDecryptStringA("-81036"), VMProtectDecryptStringA("用户名或hyxdQuick授权码或试用特征使用点数不足.")},
	{VMProtectDecryptStringA("-81037"), VMProtectDecryptStringA("当前版本不是最新版本,已被停止使用.")},
	{VMProtectDecryptStringA("-81039"), VMProtectDecryptStringA("用户电脑特征已被列入黑名单.")},
	{VMProtectDecryptStringA("-81040"), VMProtectDecryptStringA("检测用户状态过于频繁，请把检测周期设置大于等于3分钟.")},
	{VMProtectDecryptStringA("-81042"), VMProtectDecryptStringA("用户或hyxdQuick授权码提交的云数据超过最大长度.")},
	{VMProtectDecryptStringA("-81043"), VMProtectDecryptStringA("用户或hyxdQuick授权码提交的封停原因超过最大长度.")},
	{VMProtectDecryptStringA("-81044"), VMProtectDecryptStringA("提交的加入黑名单原因超过最大长度.")},
	{VMProtectDecryptStringA("-81045"), VMProtectDecryptStringA("VMP授权密钥设置错误,请查看管理端对应的设置要求进行设置.")},
	{VMProtectDecryptStringA("-81047"), VMProtectDecryptStringA("提交的VMP机器码格式错误,长度不能小于10且不能大于200")},
	{VMProtectDecryptStringA("-81048"), VMProtectDecryptStringA("VMP授权端校验失败")},
	{VMProtectDecryptStringA("-81049"), VMProtectDecryptStringA("取用户指定类型数据错误")},
	{VMProtectDecryptStringA("-83001"), VMProtectDecryptStringA("您输入的hyxdQuick授权码似乎并不存在或不正确，请检查您是否输入正确！")},
	{VMProtectDecryptStringA("-83002"), VMProtectDecryptStringA("您输入的hyxdQuick授权码似乎不正确,长度应16位！")},
	{VMProtectDecryptStringA("-83003"), VMProtectDecryptStringA("hyxdQuick授权码已被锁定.")},
	{VMProtectDecryptStringA("-83004"), VMProtectDecryptStringA("hyxdQuick授权码类型和软件扣费模式不符.")},
	{VMProtectDecryptStringA("-83005"), VMProtectDecryptStringA("该hyxdQuick授权码所属卡类为单次卡类每个电脑只能登陆使用一张.")},
	{VMProtectDecryptStringA("-83006"), VMProtectDecryptStringA("hyxdQuick授权码已到期.")},
	{VMProtectDecryptStringA("-83007"), VMProtectDecryptStringA("hyxdQuick授权码使用点数不足.")},
	{VMProtectDecryptStringA("-83008"), VMProtectDecryptStringA("hyxdQuick授权码未在绑定的电脑上登陆.")},
	{VMProtectDecryptStringA("-83009"), VMProtectDecryptStringA("hyxdQuick授权码未在绑定的IP地址登陆.")},
	{VMProtectDecryptStringA("-83011"), VMProtectDecryptStringA("hyxdQuick授权码重绑次数超过限制.")},
	{VMProtectDecryptStringA("-83012"), VMProtectDecryptStringA("hyxdQuick授权码积分不足.")},
	{VMProtectDecryptStringA("-83013"), VMProtectDecryptStringA("hyxdQuick授权码未激活.")},
	{VMProtectDecryptStringA("-83014"), VMProtectDecryptStringA("hyxdQuick授权码IP一致无需转绑即可登陆.")},
	{VMProtectDecryptStringA("-83015"), VMProtectDecryptStringA("hyxdQuick授权码机器码一致无需转绑即可登陆.")},
	{VMProtectDecryptStringA("-83016"), VMProtectDecryptStringA("hyxdQuick授权码转绑后将到期")},
	{VMProtectDecryptStringA("-83017"), VMProtectDecryptStringA("hyxdQuick授权码转绑后点数小于0")},
	{VMProtectDecryptStringA("-82001"), VMProtectDecryptStringA("用户不存在.")},
	{VMProtectDecryptStringA("-82002"), VMProtectDecryptStringA("用户名填写错误,长度必须是6-16位字母或数字.")},
	{VMProtectDecryptStringA("-82003"), VMProtectDecryptStringA("用户密码填写错误,长度必须是6-16位字母或数字.")},
	{VMProtectDecryptStringA("-82004"), VMProtectDecryptStringA("用户超级密码填写错误,长度必须是6-16位字母或数字.")},
	{VMProtectDecryptStringA("-82005"), VMProtectDecryptStringA("用户名已存在.")},
	{VMProtectDecryptStringA("-82006"), VMProtectDecryptStringA("用户已被锁定.")},
	{VMProtectDecryptStringA("-82007"), VMProtectDecryptStringA("用户已到期.")},
	{VMProtectDecryptStringA("-82008"), VMProtectDecryptStringA("用户使用点数不足.")},
	{VMProtectDecryptStringA("-82009"), VMProtectDecryptStringA("用户未在绑定的电脑上登陆.")},
	{VMProtectDecryptStringA("-82010"), VMProtectDecryptStringA("用户未在绑定的IP地址登陆.")},
	{VMProtectDecryptStringA("-82011"), VMProtectDecryptStringA("用户注册次数超过限制.")},
	{VMProtectDecryptStringA("-82012"), VMProtectDecryptStringA("用户重绑次数超过限制.")},
	{VMProtectDecryptStringA("-82013"), VMProtectDecryptStringA("用户积分不足.")},
	{VMProtectDecryptStringA("-82014"), VMProtectDecryptStringA("用户IP一致无需转绑即可登陆.")},
	{VMProtectDecryptStringA("-82015"), VMProtectDecryptStringA("用户机器码一致无需转绑即可登陆.")},
	{VMProtectDecryptStringA("-82016"), VMProtectDecryptStringA("用户注册失败，程序停止新用户注册.")},
	{VMProtectDecryptStringA("-82017"), VMProtectDecryptStringA("用户注册失败，已开启hyxdQuick授权码注册.")},
	{VMProtectDecryptStringA("-82018"), VMProtectDecryptStringA("用户超级密码错误.")},
	{VMProtectDecryptStringA("-82019"), VMProtectDecryptStringA("用户转绑后将到期")},
	{VMProtectDecryptStringA("-82020"), VMProtectDecryptStringA("用户转绑后点数小于0")},
	{VMProtectDecryptStringA("-82021"), VMProtectDecryptStringA("用户密码错误.")},
	{VMProtectDecryptStringA("-82022"), VMProtectDecryptStringA("推荐人填写错误,长度必须是6-16位字母或数字.")},
	{VMProtectDecryptStringA("-82023"), VMProtectDecryptStringA("超过最大用户数量，联系管理员升级.")},
	{VMProtectDecryptStringA("-84001"), VMProtectDecryptStringA("充值hyxdQuick授权码填写错误,长度等于16位.")},
	{VMProtectDecryptStringA("-84002"), VMProtectDecryptStringA("充值hyxdQuick授权码不存在.")},
	{VMProtectDecryptStringA("-84003"), VMProtectDecryptStringA("充值hyxdQuick授权码已被使用.")},
	{VMProtectDecryptStringA("-84004"), VMProtectDecryptStringA("充值hyxdQuick授权码已被锁定.")},
	{VMProtectDecryptStringA("-84005"), VMProtectDecryptStringA("该hyxdQuick授权码所属卡类为单次卡类每个用户只能充值一张.")},
	{VMProtectDecryptStringA("-85001"), VMProtectDecryptStringA("试用积分不足.")},
	{VMProtectDecryptStringA("-85002"), VMProtectDecryptStringA("试用特征不存在.")},
	{VMProtectDecryptStringA("-85003"), VMProtectDecryptStringA("试用特征已锁定.")},
	{VMProtectDecryptStringA("-85004"), VMProtectDecryptStringA("试用特征无此数据.")},
	{VMProtectDecryptStringA("-85005"), VMProtectDecryptStringA("试用特征已到期.")},
	{VMProtectDecryptStringA("-85006"), VMProtectDecryptStringA("试用特征使用点数不足.")},
	{VMProtectDecryptStringA("-85007"), VMProtectDecryptStringA("试用特征填写错误,长度必须是6-16位字母或数字.")},
	{VMProtectDecryptStringA("-81060"), VMProtectDecryptStringA("超过最大可激活用户数.")},
	{VMProtectDecryptStringA("-83018"), VMProtectDecryptStringA("hyxdQuick授权码不存在或试用特征不存在或此前已被封禁.")},
	{VMProtectDecryptStringA("-82024"), VMProtectDecryptStringA("用户不存在或者密码错误或此前已被封禁.")}
	};

	inline std::string OutStr(const std::string errorCode)
	{
		auto it = errorMap.find(errorCode);
		if (it != errorMap.end()) {
			return it->second;
		}
		else {
			return errorCode;
		}
	}

	//Https_WebApi请求
	inline std::string https_request(std::string url, std::string data)
	{

		http::Request request{ url };
		const auto response = request.send(VMProtectDecryptStringA("POST"), data, {
		{VMProtectDecryptStringA("Content-Type"),VMProtectDecryptStringA("application/x-www-form-urlencoded")}
			});
		return  std::string{ response.body.begin(), response.body.end() };

	}

	//公告
	inline std::string Announcement()
	{

		const std::string bodykey = VMProtectDecryptStringA("Softid=") + softids;

		str = https_request(VMProtectDecryptStringA("http://api2.1wxyun.com/?type=1"), bodykey);

		//cout << OutStr(str) << "\n";

		return str;

	}

	//最新版本号
	inline std::string LatestVersion()
	{
		const std::string bodykey = VMProtectDecryptStringA("Softid=") + softids;

		str = https_request(VMProtectDecryptStringA("http://api2.1wxyun.com/?type=3"), bodykey);

		return OutStr(str);
	}

	//hyxdQuick授权码登录
	inline bool keys_login()
	{

		const std::string bodykey = VMProtectDecryptStringA("Softid=") + softids + VMProtectDecryptStringA("&Card=") + cards + VMProtectDecryptStringA("&Version=") + versions + VMProtectDecryptStringA("&Mac=") + Macs;

		str = https_request(VMProtectDecryptStringA("http://api2.1wxyun.com/?type=17"), bodykey);

		//验证是否登陆成功
		if (str.length() == 16)
		{
			//cout << "登录成功.\n";
			LoginToken = str;
			return true;
		}
		else
		{
			//cerr << OutStr(str) << "\n";
			return false;
		}
		return false;

	}

	inline std::string TakeVariableData(const string& softid, const string& UserName, const string& Token, const string& VariableId, const string& VariableName) {
		VMProtectBeginUltra("TakeVariableData");
		// 构造请求参数
		const std::string bodykey = VMProtectDecryptStringA("Softid=") + softids + VMProtectDecryptStringA("&UserName=") + UserName + VMProtectDecryptStringA("&Token=") + Token + VMProtectDecryptStringA("&VariableId=") + VariableId + VMProtectDecryptStringA("&VariableName=") + VariableName;
		str = https_request(VMProtectDecryptStringA("http://api2.1wxyun.com/?type=7"), bodykey);
		return str;
		VMProtectEnd();
	}



	//账号密码登录
	inline bool user_login()
	{
		const std::string bodykey = "Softid=" + softids + "&UserName="+ UserName + "&UserPwd=" + UserPwd + "&Version=" + versions + "&Mac=" + Macs;

		str = https_request("http://api2.1wxyun.com/?type=11", bodykey);

		//验证是否登陆成功
		if (str.length() == 16)
		{
			cout << "登录成功.\n";
			LoginToken = str;
			return true;
		}
		else
		{
			cerr << OutStr(str) << "\n";
			return false;
		}
		return false;
	}

	//用户注册
	inline bool user_register()
	{
		const std::string bodykey = "Softid=" + softids + "&UserName=" + UserName + "&UserPwd=" + UserPwd + "&SupPwd=" + SupPwd + "&CardPwd=" + cards + "&Mac=" + Macs + "&Recommender=" + Recommender;

		str = https_request("http://api2.1wxyun.com/?type=10", bodykey);

		//验证是否登陆成功
		if (str == "1")
		{
			cout << "注册成功.\n";
			return true;
		}
		else
		{
			cerr << OutStr(str) << "\n";
			return false;
		}
		return false;
	}

	//用户充值
	inline bool user_recharge()
	{
		const std::string bodykey = "Softid=" + softids + "&UserName=" + UserName + "&CardPwd=" + cards;

		str = https_request("http://api2.1wxyun.com/?type=12", bodykey);

		//验证是否登陆成功
		if (str.length() == 16)
		{
			cout << "充值成功.\n";
			return true;
		}
		else
		{
			cerr << OutStr(str) << "\n";
			return false;
		}
		return false;
	}

	//用户改密
	inline bool user_changepw(const std::string NewUserPwd)
	{
		const std::string bodykey = "Softid=" + softids + "&UserName=" + UserName + "&SupPwd=" + SupPwd + "&NewUserPwd" + NewUserPwd;

		str = https_request("http://api2.1wxyun.com/?type=13", bodykey);

		//验证是否登陆成功
		if (str == "1")
		{
			cout << "改密成功.\n";
			return true;
		}
		else
		{
			cerr << OutStr(str) << "\n";
			return false;
		}
		return false;
	}

	//换绑
	inline bool ChangeHWID(const bool KeyMode, const int Type)
	{
		const std::string bodykey = "Softid=" + softids + "&UserName=" + (KeyMode ? cards : UserName) + "&UserPwd=" + UserPwd + "&Type=" + std::to_string(Type) + "&Mac=" + Macs;

		str = https_request("http://api2.1wxyun.com/?type=14", bodykey);

		//验证是否登陆成功
		if (str == "1")
		{
			cout << "解绑成功.\n";
			return true;
		}
		else
		{
			cerr << OutStr(str) << "\n";
			return false;
		}
		return false;
	}

	//用户登出
	inline void LoginOut()
	{
		const std::string bodykey = "Softid=" + softids + "&UserName=" + UserName + "&Token=" + LoginToken;

		str = https_request("http://api2.1wxyun.com/?type=15", bodykey);

		//验证是否登陆成功
		if (str == "1")
		{
			cout << "改密成功.\n";
		}
		else
		{
			cerr << OutStr(str) << "\n";
		}
	}

	//用户到期时间
	inline std::string GetExpried(const bool KeyMode)
	{

		const std::string bodykey = VMProtectDecryptStringA("Softid=") + softids + VMProtectDecryptStringA("&UserName=") + (KeyMode ? cards : UserName) + VMProtectDecryptStringA("&UserPwd=") + UserPwd;
		
		str = https_request(VMProtectDecryptStringA("http://api2.1wxyun.com/?type=24"), bodykey);
		return OutStr(str);

	}
}