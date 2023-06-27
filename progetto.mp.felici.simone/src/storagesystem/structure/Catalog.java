package storagesystem.structure;

import java.util.ArrayList;
import java.util.Collection;

public class Catalog extends ShoeStorageSystem {
	
	private Collection<ShoeStorageSystem> contents = new ArrayList<>();
	
	private Catalog(String name) {
		super(name);
	}
	
	public static Catalog createCatalog(String name) {
		return new Catalog(name);
	}
		
	Collection<ShoeStorageSystem> getContents() {
		return contents;
	}

	public void addContents(ShoeStorageSystem supply) {
		contents.add(supply);
	}

	public void removeContents(ShoeStorageSystem supply) {
		contents.remove(supply);
	}
	
	@Override
	public String getInfo() {
		String info = this.getName() + " contains: \n";
		for (ShoeStorageSystem content : contents) {
			info += content.getInfo();
		}
		return info;
	}

	@Override
	public double getPrice() {
		double price = 0.0;
		for (ShoeStorageSystem content : contents) {
			price += content.getPrice();
		}
		return price;
	}
}
