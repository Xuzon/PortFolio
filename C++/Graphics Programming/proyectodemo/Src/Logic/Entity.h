#ifndef __ENTITY_H
#define __ENTITY_H

class RenderThread;
/**
 * Superclase de las entidades de la aplicación.
 *
 * Tienen un método update() para ejecutar la "lógica" y un método render()
 * para hacer el renderizado.
 */
class Entity {
public:

	virtual ~Entity() {}

	/**
	 * Método llamado para hacer la actualización en cada tick.
	 *
	 * @return Cierto si la entidad debe seguir viva en este frame. Si se devuelve
	 * falso, se eliminará de la lista de entidades en este mismo frame y ni siquiera
	 * se llamará al método render().
	 */
	virtual bool update() {
		return true;
	};

	virtual void render(RenderThread& renderThread) = 0;
};

#endif // __ENTITY_H
