/* Copyright (c) 2026. LetTheMiceFree. */

#ifndef OBSERVER_MODULE_H
#define OBSERVER_MODULE_H

/**
 * @brief PATRON OBSERVATEUR : Interface de base (Classe Abstraite).
 * Définit la méthode de mise à jour que tous les observateurs concrets doivent implémenter.
 */
class IObserver
{
public:
	/**
	 * @brief Destructeur virtuel est nécessaire pour que les destructeurs des classes implémentant
	 * IObserver fonctionnent correctement.
	 */
	virtual ~IObserver () = default;
	/**
	 * @brief Méthode appelée par le sujet (GameApp) lors d'une notification.
	 */
	virtual void OnNotify () = 0;
};

#endif /* OBSERVER_MODULE_H */
