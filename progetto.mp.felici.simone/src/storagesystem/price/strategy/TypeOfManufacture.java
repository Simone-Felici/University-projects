package storagesystem.price.strategy;

public abstract class TypeOfManufacture implements TypeOfManufactureStrategy {
	
	private int hoursOfWork;
	private double additionalPrice;

	protected TypeOfManufacture(int hoursOfWork, double additionalPrice) {
		if(hoursOfWork <= 0 || additionalPrice <= 0.0) {
			throw new IllegalArgumentException("Cannot insert negative input, " 
												+ "your input is: "
												+ hoursOfWork
												+", "
												+ additionalPrice);
		}
		this.hoursOfWork = hoursOfWork;
		this.additionalPrice = additionalPrice;
	}

	protected int getHoursOfWork() {
		return hoursOfWork;
	}
	
	protected double getAdditionalPrice() {
		return additionalPrice;
	}
}
