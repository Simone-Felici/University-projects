package storagesystem.price.strategy;

public class IndustrialShoesStrategy extends TypeOfManufacture {
	
	private IndustrialShoesStrategy(int hoursOfWork, double industrialTax) {
		super(hoursOfWork, industrialTax);
	}
	
	public static IndustrialShoesStrategy industrialWith(int hoursOfWork, double industrialTax) {
		return new IndustrialShoesStrategy(hoursOfWork, industrialTax);
	}
	
	@Override
	public double getPriceOfShoes(double price) {
		return price + (price * this.getAdditionalPrice()/100) + (this.getHoursOfWork() * 8);
	}
}
