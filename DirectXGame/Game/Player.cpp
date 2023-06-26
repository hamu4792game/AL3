#include "Game/Player.h"
#include <cassert>
#include "ImGuiManager.h"

Player::Player() {

}

Player::~Player() {

}

void Player::Initialize(std::shared_ptr<Model> model, uint32_t textureHandle, Vector3 pos) {
	//	NULLポインタチェック
	assert(model);
	//	シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
	
	//	受け取ったデータをメンバ変数に記録する
	this->model_ = model;
	this->textureHandle_ = textureHandle;
	//	ワールド変換の初期化
	this->worldTransform_.Initialize();
	this->worldTransform_.translation_ = pos;
	//	3Dレティクルのワールドトランスフォームの初期化
	worldTransform3DReticle_.Initialize();
	//	レティクル用テクスチャの取得
	uint32_t textureReticle = TextureManager::Load("scope2.png");
	//	スプライト生成
	sprite2DRetecle_.reset(Sprite::Create(textureReticle, { 640.0f,360.0f }, { 1.0f,1.0f,0.0f,1.0f }, { 0.5f,0.5f }));

}

void Player::Update(ViewProjection& viewProjection) {
	//	デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	//worldTransform_.TransferMatrix();
	//	キャラクターの移動ベクトル
	Vector3 move = {0.0f, 0.0f, 0.0f};
	//	キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;
	
	//	ジョイスティック状態取得
	if (Input::GetInstance()->GetJoystickState(0,joyState))
	{
		move.x += static_cast<float>(joyState.Gamepad.sThumbLX / SHRT_MAX * kCharacterSpeed);
		move.y += static_cast<float>(joyState.Gamepad.sThumbLY / SHRT_MAX * kCharacterSpeed);
	}
	else {
		//	押した方向で移動ベクトルを変更（左右）
		if (input_->PushKey(DIK_A)) {
			move.x -= kCharacterSpeed;
		}
		else if (input_->PushKey(DIK_D)) {
			move.x += kCharacterSpeed;
		};
		//	上下
		if (input_->PushKey(DIK_W)) {
			move.y += kCharacterSpeed;
		}
		else if (input_->PushKey(DIK_S)) {
			move.y -= kCharacterSpeed;
		};
	}

	//	回転
	Rotate();
	//	座標移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	//	移動限界座標
	const float kMoveLimitX = 34.0f;
	const float kMoveLimitY = 18.0f;
	//	範囲を超えない処理
	worldTransform_.translation_.x =
	    std::clamp(worldTransform_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldTransform_.translation_.y =
	    std::clamp(worldTransform_.translation_.y, -kMoveLimitY, kMoveLimitY);

	//	キャラクター攻撃処理
	Attack();
	//	弾更新
	for (auto i = bullets_.begin(); i != bullets_.end(); i++) {
		(*i)->Update();
	}



	//	行列更新
	worldTransform_.UpdateMatrix();

	Scope(viewProjection);

	//	キャラクターの座標を画面表示する処理
	ImGui::Begin("Player");
	ImGui::Text(
	    "player : %0.2f,%0.2f,%0.2f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	ImGui::End();

}

void Player::Draw(ViewProjection& viewProjection) {

	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//	弾更新
	for (auto i = bullets_.begin(); i != bullets_.end(); i++) {
		(*i)->Draw(viewProjection);
	}
	//model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);
}

void Player::DrawUI()
{
	sprite2DRetecle_->Draw();
}

void Player::Rotate() {
	//	回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	//	押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		//	右回転
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		//	左回転
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE) || input_->IsPressMouse(0) || (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
		//	弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		velocity = GetReticleWorldPosition() - GetWorldPosition();
		velocity = Normalize(velocity) * kBulletSpeed;

		//	速度ベクトルを自機の向きに合わせて回転させる
		//velocity = TransformNormal(velocity, worldTransform_.myMatWorld_ * worldTransform_.parent_->myMatWorld_);

		//	弾を登録する
		bullets_.push_back(std::make_unique<PlayerBullet>());
		//	今追加したものの初期化処理
		//	rbegin() 逆イテレーター
		(*bullets_.rbegin())->Initialize(model_, GetWorldPosition(), velocity);
	}
}

void Player::Scope(ViewProjection& viewProjection)
{
#pragma region マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	POINT mousePosition;
	//	マウス座標を取得する
	GetCursorPos(&mousePosition);
	//	クライアントエリア座標に取得する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	//	マウス座標を2Dレティクルに代入する
	sprite2DRetecle_->SetPosition(Vector2(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));

	//	ビューポート行列
	MyMatrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//	ビュープロジェクションビューポート合成行列
	MyMatrix4x4 matVPV = viewProjection.myMatView * viewProjection.myMatProjection * matViewport;
	//	合成行列の逆行列を計算する
	MyMatrix4x4 matInverceVPV = Inverse(matVPV);
	//	スクリーン座標
	Vector3 posNear = Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 0);
	Vector3 posFar = Vector3(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y), 1);
	//	スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInverceVPV);
	posFar = Transform(posNear, matInverceVPV);
	//	マウスレイの方向
	Vector3 mouseDirection = posNear - posFar;
	mouseDirection = Normalize(mouseDirection);
	//	カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 100.0f;
	worldTransform3DReticle_.translation_ = posNear + (mouseDirection * kDistanceTestObject);

		//	3Dレティクルの行列更新
	worldTransform3DReticle_.UpdateMatrix();

#pragma endregion
}


Vector3 Player::GetWorldPosition() const
{
	Vector3 worldPos;
	//	ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.myMatWorld_.m[3][0];
	worldPos.y = worldTransform_.myMatWorld_.m[3][1];
	worldPos.z = worldTransform_.myMatWorld_.m[3][2];
	return worldPos;
}

Vector3 Player::GetReticleWorldPosition() const
{
	Vector3 worldPos;
	//	ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform3DReticle_.myMatWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.myMatWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.myMatWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision()
{

}

void Player::SetParent(const WorldTransform* parent)
{
	worldTransform_.parent_ = parent;
}
