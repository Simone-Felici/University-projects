package ProgettoAlg;

import java.util.LinkedList;

public class AlberoLF<T> {

	private NodoLF<T> radice;
	private int numNodi;
	private int numFoglie;
	private int altezza;

	public AlberoLF() {
		radice = null;
		numNodi = 0;
		numFoglie = 0;
		altezza = 0;
	}

	public int getNodi() {

		return numNodi;
	}

	public int getNumFoglie() {

		return numFoglie;
	}

	public NodoLF<T> getRadice() {
		return radice;
	}

	public int getAltezza() {

		return altezza;
	}

	public NodoLF<T> aggiungiRadice(T info) {
		if (radice != null) { // Se la radice fosse gia presente
			return null;
		}
		numNodi = 1;
		numFoglie = 1;
		altezza = 1; // l' albero con la sola radice ha altezza 1
		// perche si contano anche i nodi esterni
		return radice = new NodoLF<T>(info);
	}

	/*
	 * Il metodo aggiungiNuova radice scambia la vecchia radice con una nuova
	 * settando la vecchia come figlia della nuova.
	 */
	public NodoLF<T> aggiungiNuovaRadice(NodoLF<T> u) { // provbare poi con info (lez 6)

		if (radice == null) {
			System.out.println("Radice non esistente");
			return null;
		}

		u.setPadre(null); // Per mettere il padre di u a null (probabilmente inutile)
		u.setFiglio(radice);
		radice.setPadre(u);
		radice = u;

		altezza++;
		numNodi++;
		return radice;

	}

	/*
	 * Il metod aggiungiNodo permette di inserire un figlio dato un nodo, lo fa
	 * settando come figlio del nodo dato e viceversa settando come padre il nodo a
	 * cui vogliamo aggiungere un figlio.
	 */
	public NodoLF<T> aggiungiNodo(NodoLF<T> u, T info) {

		NodoLF<T> v = new NodoLF<T>(info);
		u.setFiglio(v);
		v.setPadre(u);
		numNodi++;
		if (v.getLivello() > altezza) {
			altezza = v.getLivello();
		}
		if (u.getGrado() > 1) { // vuol dire che u aveva gia un figlio
			numFoglie++;
		}

		return v;
	}

	public LinkedList<T> visitaDFS() {// Visita profondita

		LinkedList<T> infoNodiAlberoProfondita = new LinkedList<T>();
		LinkedList<NodoLF<T>> pila = new LinkedList<NodoLF<T>>();
		pila.addFirst(radice);

		while (!pila.isEmpty()) {
			NodoLF<T> u = pila.removeFirst();
			infoNodiAlberoProfondita.add(u.getInfo());

			for (int i = u.getFigli().size(); i > 0; i--) {
				if (u.getFigli().get(i - 1) != null) {
					pila.addFirst(u.getFigli().get(i - 1));
				}

			}
		}

		return infoNodiAlberoProfondita;
	}

	public LinkedList<T> visitaBFS() {

		LinkedList<T> infoNodiAlberoLivelli = new LinkedList<T>();
		LinkedList<NodoLF<T>> coda = new LinkedList<NodoLF<T>>(); // Simulo la coda cojn una linked
		// list, da rivedere le lezioni precedenti per cambiare eventualmente
		// implementazione

		coda.addLast(radice);

		while (!coda.isEmpty()) {
			NodoLF<T> u = coda.removeFirst();
			infoNodiAlberoLivelli.add(u.getInfo());

			for (int i = 0; i < u.getFigli().size(); i++) {
				if (u.getFigli().get(i) != null) {
					coda.addLast(u.getFigli().get(i));
				}
			}
		}

		return infoNodiAlberoLivelli;

	}

	public String toString() {

		int k = 0;
		String[] t = { "" };
		NodoLF<T> v = radice;

		if (v == null) {

			return t[0] + "null";
		}

		t[0] += "[";
		t[0] = t[0] + v.getInfo();

		f(v, t, k);

		t[0] += "]";
		return t[0];

	}

	private String f(NodoLF<T> v, String[] t, int k) {

		if (v == null) {

			return t[0];
		}

		if (v.getFigli().isEmpty()) {
			t[0] += "[]";

		}

		for (int i = 0; i < v.getFigli().size(); i++) {
			if (i == 0) {
				t[0] += "[";
			} else {
				t[0] += ",";
			}
			t[0] += v.getFigli().get(i).getInfo();
			NodoLF<T> app = v.getFigli().get(i);
			if (!app.getFigli().isEmpty()) {
				f(app, t, k);
				t[0] += "]";

			} else {
				t[0] += "[]";

			}
		}

		return t[0];

	}

}
