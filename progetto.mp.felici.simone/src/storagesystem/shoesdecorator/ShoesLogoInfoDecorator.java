package storagesystem.shoesdecorator;

import static java.util.Objects.requireNonNull;

public class ShoesLogoInfoDecorator extends ShoesInfo {
	
private final ShoesInfo shoesInfo;
	
	public ShoesLogoInfoDecorator(String name, ShoesInfo component) {
		super(name);
		requireNonNull(component, "Component cannot be null");
		this.shoesInfo = component;
	}

	@Override
	public double getPrice() {
		return shoesInfo.getPrice() + 2.5;
	}

	@Override
	public String getInfo() {
		return shoesInfo.getInfo() + " with logo: " + this.getName();
	}
}
