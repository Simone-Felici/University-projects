package storagesystem.price.strategy;

import static org.assertj.core.api.Assertions.assertThat;
import static org.assertj.core.api.Assertions.assertThatThrownBy;
import static storagesystem.price.strategy.IndustrialShoesStrategy.industrialWith;
import static storagesystem.price.strategy.PrivateEntityShoesStrategy.privateEntityWith;
import org.junit.Test;

public class TypeOfManufactureTesting {
	
	@Test
	public void testNegativeHoursOfWork() {
		assertThatThrownBy(() -> industrialWith(0, 6.0))
		.isInstanceOf(IllegalArgumentException.class)
		.hasMessage("Cannot insert negative input, your input is: 0, 6.0");
	}
	
	@Test
	public void testBothNegativeInput() {
		assertThatThrownBy(() -> privateEntityWith(0, 0.0))
		.isInstanceOf(IllegalArgumentException.class)
		.hasMessage("Cannot insert negative input, your input is: 0, 0.0");
	}
	
	@Test
	public void testNegativeIndustrialTax() {
		assertThatThrownBy(() -> industrialWith(9, 0.0))
		.isInstanceOf(IllegalArgumentException.class)
		.hasMessage("Cannot insert negative input, your input is: 9, 0.0");
	}
	
	@Test
	public void testNegativeLabor() {
		assertThatThrownBy(() -> privateEntityWith(10, 0.0))
		.isInstanceOf(IllegalArgumentException.class)
		.hasMessage("Cannot insert negative input, your input is: 10, 0.0");
	}
	
	@Test
	public void testPrivateGetPriceOfShoes() {
		TypeOfManufactureStrategy privateTypeManufacture = privateEntityWith(10, 6.0);
		double expectedValue = 30.0 + (6.0 * 10);
		
		assertThat(privateTypeManufacture.getPriceOfShoes(30.0)).isEqualTo(expectedValue);
	}
	
	@Test
	public void testIndustrialGetPriceOfShoes() {
		TypeOfManufactureStrategy industrialTypeManufacture = industrialWith(9, 15.0);
		double expectedValue = 50.0 + (50.0 * 15.0/100.) + (9 * 8);
		
		assertThat(industrialTypeManufacture.getPriceOfShoes(50.0)).isEqualTo(expectedValue);
	}
}
