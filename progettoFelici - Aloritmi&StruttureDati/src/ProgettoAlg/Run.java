package ProgettoAlg;



public class Run {

	public static void main(String[] args) {
		
		AlberoLF<String> A = new AlberoLF<>();
		/*
		NodoLF<String> Prova = new NodoLF<String>("prova");
		Prova.ListaInfoNodiInterni();
		NodoLF<String> Sauro =A.aggiungiRadice("Sauro");
		NodoLF<String> Simone =A.aggiungiNodo(Sauro, "Simone");
		NodoLF<String> Giacomo = A.aggiungiNodo(Sauro, "Giacomo");
		NodoLF<String> Martina =A.aggiungiNodo(Giacomo, "Martina");
		NodoLF<String> Rita = new NodoLF<String>("Rita");
		
		A.aggiungiNuovaRadice(Rita);
		System.out.println(A.toString());
		System.out.println(A.getRadice().getInfo());
		System.out.println(A.getAltezza());
		System.out.println(A.getNodi());
		System.out.println(Martina.livello());
		System.out.println(Sauro.livello());
		System.out.println(Martina.getInfo());
		Simone.setInfo("Sime");
		System.out.println(Simone.getInfo());
		System.out.println(Giacomo.getPadre().getInfo());
		System.out.println(Rita.getPadre());
		System.out.println(A.visitaBFS());
		System.out.println(A.visitaDFS());
		System.out.println(A.getNumFoglie());
		*/
		NodoLF<String> Claudia =A.aggiungiRadice("Claudia");
		NodoLF<String> Marco =A.aggiungiNodo(Claudia, "Marco");
		NodoLF<String> Luca =A.aggiungiNodo(Claudia, "Luca");
		NodoLF<String> Giulia =A.aggiungiNodo(Claudia, "Giulia");
		NodoLF<String> Sivlia =A.aggiungiNodo(Marco, "Silvia");
		NodoLF<String> Ugo =A.aggiungiNodo(Marco, "Ugo");
		NodoLF<String> Andrea =A.aggiungiNodo(Giulia, "Andrea");
		NodoLF<String> Gianna =A.aggiungiNodo(Giulia, "Gianna");
		NodoLF<String> Carlo =A.aggiungiNodo(Andrea, "Carlo");
		
		System.out.println(A.toString());
		
	}

}
