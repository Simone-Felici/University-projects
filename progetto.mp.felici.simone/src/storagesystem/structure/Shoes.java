package storagesystem.structure;

import storagesystem.price.strategy.TypeOfManufactureStrategy;
import storagesystem.shoesdecorator.ShoesInfo;

public class Shoes extends ShoesInfo {
	
	private double price;
	private String description;
	private TypeOfManufactureStrategy productType;
	
	private Shoes(String name, double price, String description, TypeOfManufactureStrategy productType) {
		super(name);
		this.price = price;
		this.description = description;
		this.productType = productType;
	}
	
	public static Shoes createShoes(String name, double price, String description, TypeOfManufactureStrategy productType) {
		return new Shoes(name, price, description, productType);
	}
	
	public static Shoes createShoesWithoutDescription(String name, double price, TypeOfManufactureStrategy productType) {
		return new Shoes(name, price, "", productType);
	}

	@Override
	public String getInfo() {
		return "Shoes name: " 
				+ this.getName() 
				+ this.description
				+"\n";
	}
	
	@Override
	public double getPrice() {
		return productType.getPriceOfShoes(this.price);
	}
}
