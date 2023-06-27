package storagesystem.shoesdecorator;

import static java.util.Objects.requireNonNull;

public class ShoesSignatureInfoDecorator extends ShoesInfo {
	
	private final ShoesInfo shoesInfo;
	
	public ShoesSignatureInfoDecorator(String name, ShoesInfo component) {
		super(name);
		requireNonNull(component, "Component cannot be null");
		this.shoesInfo = component;
	}

	@Override
	public double getPrice() {
		return shoesInfo.getPrice() + 5.0;
	}

	@Override
	public String getInfo() {
		return shoesInfo.getInfo() + " signature: " + this.getName();
	}
}
