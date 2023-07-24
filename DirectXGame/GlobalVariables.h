#pragma once
#include <variant>
#include "Utility.h"
#include <map>
#include <string>

class GlobalVariables {
public:
	static GlobalVariables* GetInstance();

	//項目
	struct Item {
		//項目値
		std::variant<int32_t, float, Vector3> value;
	};

	//グループ
	struct Group {
		std::map<std::string, Item> items;
	};

	//全データ
	std::map<std::string, Group> datas_;

	//グループの作成
	void CreateGroup(const std::string& groupName);

	//値のセット(int)
	void SetValue(const std::string& grouName, const std::string& key, int32_t value);
	// 値のセット(float)
	void SetValue(const std::string& grouName, const std::string& key, float value);
	// 値のセット(Vector3)
	void SetValue(const std::string& grouName, const std::string& key, Vector3& value);

	//毎フレーム処理
	void Updeat();

private:

	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables& obj) = delete;
	GlobalVariables& operator=(const GlobalVariables& obj) = delete;

	

};

