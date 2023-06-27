package storagesystem.structure;

public abstract class ShoeStorageSystem {
	
	private String name;

	public ShoeStorageSystem(String name) {
		this.name = name;
	}

	protected String getName() {
		return name;
	}
	
	public abstract String getInfo();

	public abstract double getPrice();
}
