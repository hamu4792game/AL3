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
	uint32_t textureReticle = TextureManager::Load("tex1.png");
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
	//	押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	};
	//	上下
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	};
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


	//	自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//	自機から3Dレティクルへのオフセット(z+向き)
	Vector3 offset = { 0.0f,0.0f,1.0f };
	//	自機のワールド行列の回転を反映
	offset = TransformNormal(offset, worldTransform_.myMatWorld_);
	//	ベクトルの長さを整える（正規化）
	offset = Normalize(offset) * kDistancePlayerTo3DReticle;
	//	3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = GetWorldPosition() + offset;
	//	3Dレティクルの行列更新
	worldTransform3DReticle_.UpdateMatrix();

	//	3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = GetReticleWorldPosition();
	//	ビューポート行列
	MyMatrix4x4 matViewport = MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
	//	ビュー行列とプロジェクション行列、ビューポート行列を合成する
	MyMatrix4x4 matViewProjectionViewport = viewProjection.myMatView * viewProjection.myMatProjection * matViewport;
	//	ワールドからスクリーン座標へ変換（ここで2Dから3Dになる）
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	//	スプライトのレティクルに座標設定
	sprite2DRetecle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));


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
	model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);

}

void Player::DrawUI()
{
	sprite2DRetecle_->Draw();
}

void Player::Rotate() {
	//	回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;
	//	押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		//	右回転
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		//	左回転
		worldTransform_.rotation_.y += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
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
