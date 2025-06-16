#include<stdio.h>
#include<stdlib.h>

int main(){
	int i,
		j,
		jumlah_supply,
		jumlah_demand,
		flag,
		p,
		q,
		total_supply = 0,
		total_demand = 0;
	char dummy;

	FILE *in;
	in=fopen("10x10.txt","r");

	int coordinat;
	int jumlah_ER;
	int temp;
	int coorselisih;
	int total_alloc = 0; // Inisialisasi total_alloc

	while(!feof(in)){
		fscanf(in,"%d", &jumlah_supply);
		fscanf(in,"%d", &jumlah_demand);

		int cost[jumlah_supply][jumlah_demand];
		int alloc[jumlah_supply][jumlah_demand];
		int temp_cost[jumlah_supply][jumlah_demand];
		int allocation[jumlah_supply][jumlah_demand];
		int total_alloc_baris[jumlah_supply];
		int supply_awal[jumlah_supply];


		int supply[jumlah_supply];
		int demand[jumlah_demand];
		int arr[jumlah_supply];
		int satisfy[jumlah_supply];
		double average_cost[jumlah_supply];
		double average_cost_multiply_supply[jumlah_supply];

		int flc[jumlah_demand];
		int slc[jumlah_demand];
		int diff[jumlah_demand];
		int konflik[jumlah_demand];
		int itertemp = 0;

		// input cost, supply, dan demand
		for(i=0; i<jumlah_supply; i++){
			average_cost[i] = 0;
			// Input Cost
			for(j=0; j<jumlah_demand; j++){
				fscanf(in,"%d", &cost[i][j]);
				temp_cost[i][j] = cost[i][j];
				average_cost[i] += cost[i][j];
			}
			average_cost[i] = average_cost[i] / jumlah_demand;
			printf("average_cost[%d] = %.2f\n", i, average_cost[i]);
		}

		//Input Supply
		for(i=0; i<jumlah_supply; i++){
			fscanf(in,"%d", &supply[i]);
			total_supply += supply[i];
			supply_awal[i] = supply[i];
			average_cost_multiply_supply[i] = average_cost[i] * supply[i];
			printf("average_cost_multiply_supply[%d] = %.2f\n", i, average_cost_multiply_supply[i]);
		}

		//Input Demand
		for(i=0; i<jumlah_demand; i++){
			fscanf(in,"%d", &demand[i]);
			total_demand += demand[i];
		}

		//Set alokasi semua cell menjadi -1
		for(i=0; i<jumlah_supply; i++){
			for(j=0; j<jumlah_demand; j++){
				allocation[i][j] = -1;
			}
		}

		printf("Jumlah Supply: %d\n", jumlah_supply);
		printf("Jumlah Demand: %d\n", jumlah_demand);

		/* PRINT TABEL AWAL */
		printf("\n\n(Tabel Awal)");
		printf("\n--------------------------------------------\n");
		for(p=0; p<jumlah_supply; p++){
			for(q=0; q<jumlah_demand; q++){
				if(allocation[p][q] == -1){
					printf("%d\t", cost[p][q]);
				} else {
					printf("%d\t", allocation[p][q]);
				}
			}
			printf("| %d\n", supply[p]);
		}

		for(q=0; q<jumlah_demand; q++){
			printf("_______\t");
		}

		printf("\n");

		for(q=0; q<jumlah_demand; q++){
			printf("%d\t",demand[q]);
		}

		printf("\n--------------------------------------------\n\n");

		for(p=0; p<jumlah_supply; p++){
			for(q=0; q<jumlah_demand; q++){
				alloc[p][q]=0;
			}
		}	// Loop di atas berfungsi untuk mereserve tempat untuk nilai alokasi pada cost mendatang

		// Cari cost terkecil pada tiap kolom ///

    // MODIFIKASI POIN 1
		for(p=0; p<jumlah_demand; p++){
			coordinat = 0;

			for(q=0; q<jumlah_supply; q++){
					if(cost[coordinat][p] > cost[q][p]){
						coordinat = q;
					// }
					} else if(cost[coordinat][p] == cost[q][p]){
            if(average_cost_multiply_supply[coordinat] < average_cost_multiply_supply[q]){
              coordinat = q;
							printf("coordinat %d, %d\n", coordinat, p);
            }
          }
					printf("%d\t", cost[q][p]);
			}
			printf("lokasi min : %d,%d\n", coordinat,p);
			alloc[coordinat][p] = demand[p];
		}

		while(1){
			printf("ITER %d\n", itertemp);
			jumlah_ER = 0;

			//Print tabel alokasi
			printf("\n\n");

			for(p=0;p<jumlah_supply;p++){
				arr[p] = 0;
				satisfy[p] = 0;
				total_alloc_baris[p] = 0;

				for(q=0; q<jumlah_demand; q++){
					printf("%d [%d]\t", cost[p][q], alloc[p][q]);
					total_alloc_baris[p] = total_alloc_baris[p] + alloc[p][q];
				}

				printf("| %d",supply[p]);
				printf("\t");
				printf("T. Alloc : %d\t", total_alloc_baris[p]);
        printf("AC * S = %.2f", average_cost_multiply_supply[p]);
        printf("\t");

				if(total_alloc_baris[p] > supply[p]){
					printf("Status = ER\t");
					arr[p] = 1;

					jumlah_ER++;
				} else if(total_alloc_baris[p] == supply[p]){
					printf("Status = SATISFY");
					satisfy[p] = 1;
				}
				printf("\n");
			}
			printf("\n");

			///// Cek jika ER sudah Habis /////
			if(jumlah_ER==0){
				printf("Sudah habis");
				break;
			}
			itertemp++;

			if(itertemp >= 20){ // Batas iterasi untuk menghindari infinite loop
				printf("ITER LEWATTTTT");
				break;
			} else if (jumlah_ER>0){ //Jika baris ada yang ER
				for(q=0; q<jumlah_demand; q++){
					konflik[q]=0;

					for(p=0; p<jumlah_supply; p++){
						if(alloc[p][q]>0 && arr[p] == 1){
							konflik[q] = 1;
							// printf("Kolom %d konflik.\n", q);
							break;
						}
					}
				}

				// Menghitung FLC
				for(q=0; q<jumlah_demand; q++){
					if(konflik[q] == 1){
						int iter = 0;

						for(p=0; p<jumlah_supply; p++){
							iter++;

							if(satisfy[p] != 1 && alloc[p][q] > 0){
								temp = cost[p][q];
								flc[q] = p;

								break;
							}
						}

						for(p=iter; p<jumlah_supply; p++){
							if(satisfy[p] !=1 && temp > cost[p][q] && alloc[p][q] > 0){
								temp = cost[p][q];
								flc[q] = p;
							}
						}
					}
				}

				// Menghitung SLC
				for(q=0; q<jumlah_demand; q++){
					if(konflik[q] == 1){
						int iter = 0;

						for(p=0; p<jumlah_supply; p++){
							iter++;

							if(flc[q]!=p && arr[p] != 1 && satisfy[p] != 1){
								temp = cost[p][q];
								slc[q] = p;

								break;
							}
						}

						for(p=iter; p<jumlah_supply; p++){
							if(flc[q]!=p && arr[p] != 1 && satisfy[p] != 1){
								if(temp > cost[p][q]){
										temp = cost[p][q];
										slc[q] = p;
								} else if(temp == cost[p][q] && average_cost_multiply_supply[q] < average_cost_multiply_supply[p]){
										slc[q] = p;
								}
							}
						}
					}
          printf("FLC[%d] = %d, SLC[%d] = %d\n", q, flc[q], q, slc[q]);
				}

				printf("\n");

				// Menyimpan diff tiap kolom
				for(q=0; q<jumlah_demand; q++){
					if(konflik[q] == 1){
						diff[q] = cost[slc[q]][q] - cost[flc[q]][q];
            			printf("diff[%d] = %d\n", q, diff[q]);
					}
				}

				int iter = 0;

				for(q=0; q<jumlah_demand; q++){
					iter++;

					if(konflik[q]==1){
						coorselisih = q;
						break;
					}
				}

				for(q=iter; q<jumlah_demand; q++){
					if(konflik[q] == 1){
						if(diff[coorselisih] == diff[q]){
							if(alloc[flc[coorselisih]][coorselisih] < alloc[flc[q]][q]){
								coorselisih = q;
							}
						} else if(diff[coorselisih] > diff[q]){
							coorselisih = q;
						}
					}
				}

				printf("Selisih terkecil ada di kolom ke : %d\n",coorselisih);

				int baris_slc = slc[coorselisih];
				int baris_flc = flc[coorselisih];

				// CHANGE PARAMETER SUPPLY TO AVERAGE COST * S
				if(average_cost_multiply_supply[baris_flc] == average_cost_multiply_supply[baris_slc]){	//Jika ada supply yg sama
					printf("Sama");

					if(total_alloc_baris[baris_flc] - alloc[baris_flc][coorselisih] <= supply[baris_flc]){
						alloc[baris_flc][coorselisih] -= (total_alloc_baris[baris_flc] - supply[baris_flc]);
						alloc[baris_slc][coorselisih] += (total_alloc_baris[baris_flc] - supply[baris_flc]);
					} else if (total_alloc_baris[baris_slc] + alloc[baris_flc][coorselisih] >= supply[baris_slc]){
						alloc[baris_slc][coorselisih] += (supply[baris_slc] - total_alloc_baris[baris_slc]);
						alloc[baris_flc][coorselisih] -= (supply[baris_slc] - total_alloc_baris[baris_slc]);
					} else {// Jika tidak bisa satisfy second
						alloc[baris_slc][coorselisih] += alloc[baris_flc][coorselisih];
						alloc[baris_flc][coorselisih] = 0;
					}
				} else if(jumlah_ER >= 2){
					if(average_cost_multiply_supply[baris_flc] > average_cost_multiply_supply[baris_slc]){
						printf("satisfy supply baris index %d dulu",baris_flc);

						if(total_alloc_baris[baris_flc] - alloc[baris_flc][coorselisih] <= supply[baris_flc]){
							alloc[baris_flc][coorselisih] -=  (total_alloc_baris[baris_flc] - supply[baris_flc]);
							alloc[baris_slc][coorselisih] +=  (total_alloc_baris[baris_flc] - supply[baris_flc]);
						} else {//else if (Ta - alloc > Supply)
							alloc[baris_slc][coorselisih] += alloc[baris_flc][coorselisih];
							alloc[baris_flc][coorselisih] = 0;
						}
					} else {
						printf("satisfy supply baris index %d dulu - else",baris_slc);

						int alloc_sisa = supply[baris_slc]-total_alloc_baris[baris_slc];

						if( alloc_sisa >= alloc[baris_flc][coorselisih]){
							printf("lebihbesar\n");
							alloc[baris_slc][coorselisih] += alloc[baris_flc][coorselisih];
							alloc[baris_flc][coorselisih] = 0;
						} else {
							printf("lebihkecil\n");
							alloc[baris_slc][coorselisih] = alloc_sisa;
							alloc[baris_flc][coorselisih] -= alloc_sisa;
						}
					}
				} else {//Jika ER=1
					printf("ER Lebih Kecil \n");

					if(average_cost_multiply_supply[baris_flc] < average_cost_multiply_supply[baris_slc]){
						printf("satisfy supply baris index %d dulu",baris_flc);

						if(total_alloc_baris[baris_flc] - alloc[baris_flc][coorselisih] <= supply[baris_flc]){
							alloc[baris_flc][coorselisih] -=  (total_alloc_baris[baris_flc] - supply[baris_flc]);
							alloc[baris_slc][coorselisih] +=  (total_alloc_baris[baris_flc] - supply[baris_flc]);

							printf("hey");
						} else {//else if (Ta - alloc > Supply)
							alloc[baris_slc][coorselisih] += alloc[baris_flc][coorselisih];
							alloc[baris_flc][coorselisih] = 0;
						}
					} else {
						printf("satisfy supply baris index %d dulu - else",baris_slc);

						int alloc_sisa = supply[baris_slc]-total_alloc_baris[baris_slc];

						if( alloc_sisa >= alloc[baris_flc][coorselisih]){
							printf("lebihbesar\n");

							alloc[baris_slc][coorselisih] += alloc[baris_flc][coorselisih];
							alloc[baris_flc][coorselisih] = 0;
						} else {
							printf("lebihkecil\n");

							alloc[baris_slc][coorselisih] = alloc_sisa;
							alloc[baris_flc][coorselisih] -= alloc_sisa;
						}
					}
				}
			}
		}

		jumlah_ER = 0;

		printf("\n\n");

		for(p=0; p<jumlah_supply; p++){
			total_alloc_baris[p] = 0;

			for(q=0; q<jumlah_demand; q++){
				printf("%d [%d]\t", cost[p][q], alloc[p][q]);
				total_alloc_baris[p] = total_alloc_baris[p] + alloc[p][q];
			}

			printf("| %d",supply[p]);
			printf("\t");
			printf("Total alloc : %d\t", total_alloc_baris[p]);

			if(total_alloc_baris[p] > supply[p]){
				printf("Status : ER\t");

				arr[p] = 1;
				jumlah_ER++;
			}
			printf("\n");
		}

		// Hitung total biaya
		total_alloc=0; // Reset total_alloc sebelum menghitung ulang
		for(p=0; p<jumlah_supply; p++){
			for(q=0; q<jumlah_demand; q++){
				total_alloc += cost[p][q] * alloc[p][q];
			}
		}
		printf("\nJumlah iterasi yang dijalankan: %d\n", itertemp); // Output jumlah iterasi
		printf("Total Alokasi tabel adalah: %d\n", total_alloc);
	}
}
