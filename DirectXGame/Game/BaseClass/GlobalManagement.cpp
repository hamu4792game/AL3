#include "GlobalManagement.h"
#include <imgui.h>

GlobalManagement* GlobalManagement::GetInstance()
{
	static GlobalManagement management;
	return &management;
}

void GlobalManagement::CreateGroup(const std::string& groupName)
{
	//	指定名のオブジェクトがなければ追加する
	datas_[groupName];
}

void GlobalManagement::SetValue(const std::string& groupName, const std::string& key, int32_t value)
{
	//	グループの参照を取得
	Group& group = datas_[groupName];
	//	新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	//	設定した項目をstd::mapに追加
	group.items[key] = newItem;

}

void GlobalManagement::SetValue(const std::string& groupName, const std::string& key, float value)
{
	//	グループの参照を取得
	Group& group = datas_[groupName];
	//	新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	//	設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalManagement::SetValue(const std::string& groupName, const std::string& key, const Vector3& value)
{
	//	グループの参照を取得
	Group& group = datas_[groupName];
	//	新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	//	設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

void GlobalManagement::Updata()
{
	if (!ImGui::Begin("Global Management",nullptr,ImGuiWindowFlags_MenuBar))
	{
		//	メニューバーを使用可能なフラグを付けてウィンドウを開く
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) return;

	//	各グループについて
	for (/*std::unordered_map<std::string, Group>::iterator*/
		auto itGroup = datas_.begin(); itGroup != datas_.end(); ++itGroup)
	{
		//	グループ名を取得 first = キー
		const std::string& groupName = itGroup->first;
		//	グループの参照を取得 second = 値
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) continue;
		//	各項目について
		for (auto itItem = group.items.begin(); itItem != group.items.end(); ++itItem) {
			//	項目名を取得
			const std::string& itemName = itItem->first;
			//	項目の参照を取得
			Item& item = itItem->second;

			//	int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item)) {
				int32_t* ptr = std::get_if<int32_t>(&item);
				ImGui::SliderInt(itemName.c_str(), ptr, 0, 100);
			}
			else if (std::holds_alternative<float>(item)) {
				float* ptr = std::get_if<float>(&item);
				ImGui::SliderFloat(itemName.c_str(), ptr, 0, 100);
			}
			else if (std::holds_alternative<Vector3>(item)) {
				Vector3* ptr = std::get_if<Vector3>(&item);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f, 10.0f);
			}
		}
		ImGui::EndMenu();
			
	}
	ImGui::EndMenuBar();
	ImGui::End();

}
