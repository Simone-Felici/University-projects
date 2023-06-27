package ProgettoAlg;

import java.util.LinkedList;

public class NodoLF<T> {

	private T info;
	private NodoLF<T> padre;
	private LinkedList<NodoLF<T>> figli;

	public NodoLF(T dato) {
		this.info = dato;
		padre = null;
		figli = new LinkedList<NodoLF<T>>();

	}

	public T getInfo() {
		return info;
	}

	public void setInfo(T info) {
		this.info = info;
	}

	public NodoLF<T> getPadre() {
		return padre;
	}

	public void setPadre(NodoLF<T> padre) {
		this.padre = padre;
	}

	public LinkedList<NodoLF<T>> getFigli() {
		return figli;
	}

	public void setFiglio(NodoLF<T> u) {

		figli.add(u);
	}

	/*il metodo getGrado va a contare il numero di figli 
	 *di un nodo */
	public int getGrado() {
		
		return this.figli.size();
	}

	/*il metodo ListaInfoNodiInterni mi ritorna 
	 * un linked list di informazioni dei nodi 
	 * che sono figli del nodo su cui richiamo 
	 * il metodo. */
	public LinkedList<T> ListaInfoNodiInterni() {

		LinkedList<T> r = new LinkedList<T>();
		for (int i = 0; i < figli.size(); i++) {

			if (figli.get(i) != null) {
				r.add(figli.get(i).getInfo());
			}

		}
		return r;
	}

	/*Il metodo getLivello parte dal nodo su
	 * cui richiamo il metodo e se il suo nodo
	 * padre non è null allora sale a quel nodo,
	 * così fino a qando non trova piu un nodo 
	 * padre, ovvero la radice
	 */
	public int getLivello() {

		int livello = 0;
		NodoLF<T> temp = padre;
		while (temp != null) {
			livello++;
			temp = temp.getPadre();
		}
		return livello;
	}

}
