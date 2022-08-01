#pragma once

class Utils
{
public:
	template <typename T>
	T*	GetComponentFromPlayer(APawn* Pawn) const;

	
};

template<typename T>
T*	Utils::GetComponentFromPlayer(APawn* Pawn) const
{
	const auto Player = Pawn->GetOwningPlayerPawn();
	return Player == nullptr ? nullptr : Cast<T>(Player->GetComponentByClass(T::StaticClass()));
}