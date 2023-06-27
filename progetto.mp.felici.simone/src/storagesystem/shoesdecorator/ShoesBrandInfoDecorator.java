package storagesystem.shoesdecorator;

import static java.util.Objects.requireNonNull;

public class ShoesBrandInfoDecorator extends ShoesInfo {

	private final ShoesInfo shoesInfo;

	public ShoesBrandInfoDecorator(String name, ShoesInfo component) {
		super(name);
		requireNonNull(component, "Component cannot be null");
		this.shoesInfo = component;
	}

	@Override
	public double getPrice() {
		return shoesInfo.getPrice() + 10.0;
	}

	@Override
	public String getInfo() {
		return shoesInfo.getInfo() + " of brand: " + this.getName();
	}
}
