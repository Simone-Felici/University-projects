package storagesystem.structure;

import static org.assertj.core.api.Assertions.assertThat;
import static storagesystem.price.strategy.IndustrialShoesStrategy.industrialWith;
import static storagesystem.price.strategy.PrivateEntityShoesStrategy.privateEntityWith;
import static storagesystem.structure.Shoes.*;
import org.junit.Test;

import storagesystem.price.strategy.TypeOfManufactureStrategy;

public class ShoesTesting {
	
	@Test
	public void testsGetPriceIndustrial() {
		TypeOfManufactureStrategy industrialType = industrialWith(9, 15.0);
		Shoes shoes = createShoesWithoutDescription("Puma 1980", 70.0, industrialType);
		double expectedPrice = 70.0 + (70*15.0/100) + (9*8);
		
		assertThat(shoes.getPrice()).isEqualTo(expectedPrice);
	}
	
	@Test
	public void testsGetPricePrivateEntity() {
		TypeOfManufactureStrategy privateType = privateEntityWith(10, 6.0);
		Shoes shoes = createShoesWithoutDescription("Puma 1980", 70.0, privateType);
		double expectedPrice = 70.0 + (6.0 * 10);
		
		assertThat(shoes.getPrice()).isEqualTo(expectedPrice);
	}

	@Test
	public void testsGetInfo() {
		TypeOfManufactureStrategy industrialType = industrialWith(3, 5.0);
		Shoes shoes = createShoes("NewBalance",
									55.0,
									"Tip:round; Closure:laces; Material:fabric;",
									industrialType);
		
		assertThat(shoes.getInfo()).isEqualTo("Shoes name: NewBalance"
												+ "Tip:round; Closure:laces; Material:fabric;"
												+ "\n");
	}
	
	@Test
	public void testsGetInfoWithoutDescription() {
		TypeOfManufactureStrategy industrialType = industrialWith(3, 5.0);
		Shoes shoes = createShoesWithoutDescription("NewBalance", 55.0, industrialType);
		
		assertThat(shoes.getInfo()).isEqualTo("Shoes name: "
												+ "NewBalance"
												+ "\n");
	}
}
