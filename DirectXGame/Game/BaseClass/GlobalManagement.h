#include <iostream>
#include <variant>
#include <string>
#include <unordered_map>
#include "math/Vector3.h"

/// <summary>
/// グローバル変数 調整項目用クラス
/// </summary>
class GlobalManagement {
public:
	//	シングルトンインスタンス
	static GlobalManagement* GetInstance();
private:
	GlobalManagement() = default;
	~GlobalManagement() = default;
	GlobalManagement(const GlobalManagement&) = delete;
	GlobalManagement& operator=(const GlobalManagement&) = delete;

private:
	//	項目
	using Item = std::variant<int32_t, float, Vector3>;
	//	グループ
	struct Group {
		std::unordered_map<std::string, Item> items;
	};
	//	全データ
	std::unordered_map<std::string, Group> datas_;

public:
	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	//	値のセット(int)
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	//	値のセット(float)
	void SetValue(const std::string& groupName, const std::string& key, float value);
	//	値のセット(Vector3)
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Updata();

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);

public:
	//	グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath = "Resources/GlobalManagement/";

};
