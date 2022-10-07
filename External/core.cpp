#include "main.cpp"
#include "define.h"


namespace Core {
	bool NoSpread = true;
	PVOID LocalPlayerPawn = nullptr;
	PVOID LocalPlayerController = nullptr;
	PVOID TargetPawn = nullptr;

	PVOID(*ProcessEvent)(PVOID, PVOID, PVOID, PVOID) = nullptr;
	PVOID(*CalculateSpread)(PVOID, float*, float*) = nullptr;
	float* (*CalculateShot)(PVOID, PVOID, PVOID) = nullptr;
	VOID(*ReloadOriginal)(PVOID, PVOID) = nullptr;
	PVOID(*GetWeaponStats)(PVOID) = nullptr;
	INT(*GetViewPoint)(PVOID, FMinimalViewInfo*, BYTE) = nullptr;

	PVOID calculateSpreadCaller = nullptr;
	float originalReloadTime = 0.0f;

	BOOLEAN GetTargetHead(FVector& out) {
		if (!Core::TargetPawn) {
			return FALSE;
		}

		auto mesh = ReadPointer(Core::TargetPawn, 0x278);
		if (!mesh) {
			return FALSE;
		}

		auto bones = ReadPointer(mesh, 0x420);
		if (!bones) bones = ReadPointer(mesh, 0x420 + 0x10);
		if (!bones) {
			return FALSE;
		}

		float compMatrix[4][4] = { 0 };
		Util::ToMatrixWithScale(reinterpret_cast<float*>(reinterpret_cast<PBYTE>(mesh) + 0x1C0), compMatrix);

		Util::GetBoneLocation(compMatrix, bones, 66, &out.X);
		return TRUE;
	}

	PVOID ProcessEventHook(UObject* object, UObject* func, PVOID params, PVOID result) {
		if (object && func) {
			auto objectName = Util::GetObjectFirstName(object);
			auto funcName = Util::GetObjectFirstName(func);

			do {
				if (Core::TargetPawn && Core::LocalPlayerController) {
					if (wcsstr(objectName.c_str(), L"B_Prj_Bullet_Sniper") && funcName == L"OnRep_FireStart") {
						FVector head = { 0 };
						if (!GetTargetHead(head)) {
							break;
						}

						*reinterpret_cast<FVector*>(reinterpret_cast<PBYTE>(object) + Offsets::FortniteGame::FortProjectileAthena::FireStartLoc) = head;

						auto root = reinterpret_cast<PBYTE>(ReadPointer(object, Offsets::Engine::Actor::RootComponent));
						*reinterpret_cast<FVector*>(root + Offsets::Engine::SceneComponent::RelativeLocation) = head;
						memset(root + Offsets::Engine::SceneComponent::ComponentVelocity, 0, sizeof(FVector));
					}
					else if (!Settings.SilentAimbot && wcsstr(funcName.c_str(), L"Tick")) {
						FVector head;
						if (!GetTargetHead(head)) {
							break;
						}

						float angles[2] = { 0 };
						Util::CalcAngle(&Util::GetViewInfo().Location.X, &head.X, angles);

						if (Settings.AimbotSlow <= 0.0f) {
							FRotator args = { 0 };
							args.Pitch = angles[0];
							args.Yaw = angles[1];
							ProcessEvent(Core::LocalPlayerController, Offsets::Engine::Controller::SetControlRotation, &args, 0);
						}
						else {
							auto scale = Settings.AimbotSlow + 1.0f;
							auto currentRotation = Util::GetViewInfo().Rotation;

							FRotator args = { 0 };
							args.Pitch = (angles[0] - currentRotation.Pitch) / scale + currentRotation.Pitch;
							args.Yaw = (angles[1] - currentRotation.Yaw) / scale + currentRotation.Yaw;
							ProcessEvent(Core::LocalPlayerController, Offsets::Engine::Controller::SetControlRotation, &args, 0);
						}
					}
				}
			} while (FALSE);
		}

		return ProcessEvent(object, func, params, result);
	}
