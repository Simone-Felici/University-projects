package storagesystem.price.strategy;

public class PrivateEntityShoesStrategy extends TypeOfManufacture {
	
	private PrivateEntityShoesStrategy(int hoursOfWork, double labor) {
		super(hoursOfWork, labor);
	}
	
	public static PrivateEntityShoesStrategy privateEntityWith(int hoursOfWork, double labor) {
		return new PrivateEntityShoesStrategy(hoursOfWork, labor);
	}

	@Override
	public double getPriceOfShoes(double price) {
		return price + this.getAdditionalPrice() * this.getHoursOfWork();
	}

}
